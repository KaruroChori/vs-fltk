#include <condition_variable>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <optional>
#include <thread>
#include <mutex>
#include <vector>

#include <iostream>
#include <pugixml.hpp>

//Progressive Unique ID
struct puid{
    static inline uint64_t next=0;
    static constexpr int SIZE = 1;
    uint64_t data;
    puid(){data=next++;}
};

//Uniform Unique ID
struct uuid{
    static constexpr int SIZE = 8;
    uint64_t data[SIZE];

    friend bool operator<(const uuid&a, const uuid&b){
        for(int i=0;i<SIZE;i++)if(a.data[i]<b.data[i])return true;
        return false;
    }

    void set_random(){
        //TODO: Use proper crypto functions
        for(int i=0;i<SIZE;i++)data[i]=random();
    }

    void set_zero(){
        //TODO: Use proper crypto functions
        for(int i=0;i<SIZE;i++)data[i]=0;
    }

    void mix(const uuid& ref){
        //TODO: Replace with a real function
        for(int i=0;i<SIZE;i++)data[i]^=ref.data[i];
    }

    std::vector<uint8_t> to_buffer(){
        const char* buffer = (const char*)&data;
        return std::vector<uint8_t>(buffer,buffer+SIZE*8);
    }

    uuid() = default;
    uuid(const char* str){*this=str;}
    uuid& operator=(const char* str){
        uint8_t* buffer = (uint8_t*)&data;
        set_zero();
        for(int i=0,c=0;c<SIZE*8*2;i++){
            if(str[i]>='a'&&str[i]<='f'){buffer[c/2]|=((str[i]-'a')+10)<<((c%2==1)*4);c++;}
            else if(str[i]>='A'&&str[i]<='F'){buffer[c/2]|=((str[i]-'A')+10)<<((c%2==1)*4);c++;}
            else if(str[i]>='0'&&str[i]<='9'){buffer[c/2]|=((str[i]-'0'))<<((c%2==1)*4);c++;}
            else if(str[i]==0){
                //For safety reasons, malformed input will just generate random values.
                set_random();
                return *this;
            }
            else{}
        }
        return *this;
    }

};

/*
    Utility logger for resource handling.
*/
class res_logger_t{
    private:

    public:
        static inline  FILE* channel = stdout;

        enum msg_t{
            NEEDED,
            WAIT_FOR,
            PRODUCED,
            INVALID_CACHE,
            GOOD_CACHE,
            FAILED
        };

        void operator()(msg_t severety, const std::string& path, const char* str, ...){
            static const char* msg_table[] = {
                "\033[37;1m[!]\033[0m ",
                "\033[90;1m[…]\033[0m ",
                "\033[32;1m[>]\033[0m ",
                "\033[36;1m[M]\033[0m ",
                "\033[34;1m[T]\033[0m ",
                "\033[31;1m[✖]\033[0m ",
            };

                std::string rstr = std::string("\n")+std::string(msg_table[(int)severety%6]) + std::string(str) + " @ [\033[93;3m" + path + "\033[0m]";
    
                va_list args;
                va_start(args, str);
                vfprintf(channel,rstr.c_str(), args);
                va_end(args);

                fflush(stdout);
        }
};

/*
    Class containing all the cache models used in the application.
*/
class cache_model_base{
    public:

    template<typename T>
    struct cache_class{
        res_logger_t log;
        struct key_t{
            uint64_t hash;
            uuid channel;
        };
        struct entry_t{
            uint64_t timestamp;
            std::vector<uint8_t> name;
            T entry;
            //entry_t& operator=(entry_t&&){return *this;}
        };

        struct cmp_key {
            bool operator()(const key_t& a, const key_t& b) const {
                if(a.hash<b.hash)return true;
                else if (a.channel<b.channel)return true;
                return false;
            }
        };

        virtual void set(const std::vector<uint8_t>& name, T&& entry, const uuid& channel={}) noexcept =0;
        virtual entry_t* get(const std::vector<uint8_t>& name, const uuid& channel={}) noexcept = 0;
        virtual void invalidate(const std::vector<uint8_t>& name, const uuid& channel={}) noexcept = 0;

        inline  const entry_t* get(const std::vector<uint8_t>& name, const uuid& channel={}) const noexcept {return get(name,channel);}
        virtual ~cache_class(){}
    };

    protected:

    template<typename T>
    struct cache_class_memory : cache_class<T>{

        std::map<typename cache_class<T>::key_t, typename cache_class<T>::entry_t, typename cache_class<T>::cmp_key> entries;
        std::mutex entries_mtx;

        static inline uint64_t hasher(const std::vector<uint8_t>& vec) {
            //TODO
            if(vec.size()==0)return 0x134f44;
            return vec[vec[0]%vec.size()]+(vec[vec[3%vec.size()]%vec.size()]<<8)+(vec[vec[5%vec.size()]%vec.size()]<<16)+(vec[vec[7%vec.size()]%vec.size()]<<24);
        };

        public:
            virtual void set(const std::vector<uint8_t>& name, T&& entry, const uuid& channel={}) noexcept{
                std::lock_guard<std::mutex> guard(entries_mtx);
                try{
                    auto hash = hasher(name);
                    entries.erase(typename cache_class<T>::key_t{hash,channel});
                    entries.emplace(typename cache_class<T>::key_t{hash,channel},
                                    typename cache_class<T>::entry_t{(uint64_t)std::chrono::system_clock::now().time_since_epoch().count(),name,std::move(entry)});

                }
                catch(...){}
            }
            virtual typename cache_class<T>::entry_t* get(const std::vector<uint8_t>& name, const uuid& channel={}) noexcept {
                std::lock_guard<std::mutex> guard(entries_mtx);
                try{
                    auto it = entries.find({hasher(name),channel});

                    if(it!=entries.end())return &it->second;
                    else return nullptr;

                }
                catch(...){
                }

                return nullptr;
            }

            virtual void invalidate(const std::vector<uint8_t>& name, const uuid& channel={}) noexcept{
                std::lock_guard<std::mutex> guard(entries_mtx);
                try{
                    entries.erase({hasher(name),channel});
                }
                catch(...){
                }
            }

        virtual ~cache_class_memory(){}
    };
};


class xml_loader;
class native_loader;
class tcc;

class tcc{};

class cache_model : public cache_model_base{

    public:

    //Special one used to prepare in memory any resource being recalled via src. 
    cache_class_memory<std::string> remote_content;

    //Collections for artifacts

    cache_class_memory<std::string> collect_entries;
    //cache_class_memory<std::string> compile_entries;

    //cache_class_memory<native_loader> native_components;
    //cache_class_memory<native_loader> xml_components;

    //cache_class_memory<tcc> native_script;

}global_cache;

class native_loader{
    uuid auth;
    uint64_t depth = 0;


    //std::vector<std::thread> pool;  //Local pool of threads for collection, so that we can abort this process only.

    public:
    //native_loader(native_loader&& loader){}

    //native_loader(){}
};

class xml_loader{
    pugi::xml_document doc;
    std::string basename;           //Normalized basename, so that src can be scoped on top of it.

    uuid auth_token;
    uuid auth_session;

    uint64_t depth = 0;

    std::mutex xml_tree_mutex;      //Mutex to access the xml_tree
    std::condition_variable cv;     //To notify progress on compilation / retrival
    std::vector<std::thread> pool;  //Local pool of threads for collection, so that we can abort this process only.

    //They are split as some nodes can have both inline content and src.
    std::map<void*,uuid> cache_refs_src;    //To keep track of cache refs for artifacts generated via src loading
    std::map<void*,uuid> cache_refs_body;   //To keep track of cache refs for artifacts generated via inline content.

    //List of imports detected, used to recognize them later in the tree
    std::map<std::string,std::string> custom_tags;

    static inline cache_model& cache = global_cache;

    template<typename T>
    using collector_t =std::optional<T> (*) (const xml_loader& from, const pugi::xml_node&);

    //Collect a raw resource from somewhere to local cache.
    bool collect_external_helper(pugi::xml_node ref, collector_t<std::string> collector){
        const auto& src=ref.attribute("src").as_string();
        auto path = std::vector<uint8_t>(src,src+strlen(src));

        bool fetch_needed = false;
        auto cache_policy =ref.attribute("src.cache-policy").as_string("forever");
        bool is_session = strcmp(cache_policy,"session")==0;

        if(strcmp(cache_policy,"no-cache")==0){
            cache.remote_content.log(res_logger_t::INVALID_CACHE, src, "no-cache policy for SRC");  
            fetch_needed=true;
        }
        else {
            auto* tmp = cache.remote_content.get(path,is_session?auth_session:auth_token);
            if(tmp==nullptr){
                cache.remote_content.log(res_logger_t::INVALID_CACHE, src, "missing entry for SRC");  
                fetch_needed=true;
            }
            //Forever needs no further check 
            if(strcmp(cache_policy,"forever")==0){}
            //If a number is passed, take it as relative to stored to determine if it is still ok.
            else if((long unsigned int)ref.attribute("src.cache-policy").as_int()>std::chrono::system_clock::now().time_since_epoch().count()-tmp->timestamp){
                cache.remote_content.log(res_logger_t::INVALID_CACHE, src, "cache timeout for SRC");  
                fetch_needed=true;
            }
        }
        bool success = false;

        if(fetch_needed){
            auto tmp = collector(*this,ref);
            if(tmp.has_value()){
                cache.remote_content.set(path,std::move(tmp.value()),is_session?auth_session:auth_token);   
                success=true;
            }
            else{success=false;}
        }
        else{
            success=true;
        }
        return success;

    }
    
    void collect_external(pugi::xml_node ref, collector_t<std::string> collector){
        auto success = collect_external_helper(ref, collector);
        {
            std::lock_guard<std::mutex> guard(xml_tree_mutex);
            ref.append_attribute("src.success").set_value(success?"true":"false");
        }
        cv.notify_one();
    }

    //Collection of any resource based on the src attribute
    template <typename T, bool INLINE>
    void compile_artifact(pugi::xml_node ref, cache_model::cache_class<T>& cache_line, collector_t<T> collector){
        auto path = INLINE?ref.path():ref.attribute("src").as_string();
        std::vector<uint8_t> cache_path;
        const auto& cached_path = ref.attribute("body.cached");
        if(cached_path.empty()){
            std::lock_guard<std::mutex> guard(xml_tree_mutex);
            if(INLINE)cache_path = cache_refs_body.emplace(ref.internal_object(),uuid("")).first->second.to_buffer();
            else cache_path = cache_refs_src.emplace(ref.internal_object(),uuid("")).first->second.to_buffer();
        }
        else{
            cache_path = std::vector<uint8_t>((uint8_t*)(cached_path.as_string()),(uint8_t*)(cached_path.as_string())+uuid::SIZE*8);
        }

        bool fetch_needed = false;
        auto cache_policy =ref.attribute("body.cache-policy").as_string("forever");
        bool is_session = strcmp(cache_policy,"session")==0;

        if(strcmp(cache_policy,"no-cache")==0){
            cache_line.log(res_logger_t::INVALID_CACHE, path, "no-cache policy");  
            fetch_needed=true;
        }
        else {
            auto* tmp = cache_line.get(cache_path,is_session?auth_session:auth_token);
            if(tmp==nullptr){
                cache_line.log(res_logger_t::INVALID_CACHE, path, "missing entry");  
                fetch_needed=true;
            }
            //Forever needs no further check 
            if(strcmp(cache_policy,"forever")==0){}
            //If a number is passed, take it as relative to stored to determine if it is still ok.
            else if((long unsigned int)ref.attribute("body.cache-policy").as_int()>std::chrono::system_clock::now().time_since_epoch().count()-tmp->timestamp){
                cache_line.log(res_logger_t::INVALID_CACHE, path, "cache timeout");  
                fetch_needed=true;
            }
        }
        bool success = false;

        if(fetch_needed){
            if(!INLINE)collect_external_helper(ref,resolve_test_src);
            auto tmp = collector(*this,ref);
            if(tmp.has_value()){
                cache_line.set(cache_path,std::move(tmp.value()),is_session?auth_session:auth_token);   
                success=true;
            }
            else{success=false;}
        }
        else{
            cache_line.log(res_logger_t::GOOD_CACHE, path, "cached");
            success=true;
        }

        {
            std::lock_guard<std::mutex> guard(xml_tree_mutex);
            ref.append_attribute("body.success").set_value(success?"true":"false");
            ref.append_attribute("body.cached").set_value((const char*)cache_path.data());
        }
        cv.notify_one();
    }

    //Resolve component in use,use.xml or import,import.xml blocks
    static std::optional<xml_loader> resolve_xml_src(const xml_loader& from, const pugi::xml_node&){
        //xml_loader tmp(name.c_str(),&from);
    }

    //Resolve component in use.native or import.native blocks
    static std::optional<native_loader> resolve_native_src(const xml_loader& from, const pugi::xml_node&){
        return {};
    }
    
    //Resolve the source of a `script` block and compile it into a native resource
    static std::optional<std::shared_ptr<tcc>> native_script_src(const xml_loader& from, const pugi::xml_node&){
        return {};
    }
    
    //Compile the inline data of a `script` block into a native resource
    static std::optional<std::shared_ptr<tcc>> native_script_content(const xml_loader& from, const pugi::xml_node&){
        return {};
    }
    
    //Resolve the source of an `immutable` data block.
    static std::optional<std::shared_ptr<tcc>> string_src(const xml_loader& from, const pugi::xml_node&){
       return {};
    }

    static std::optional<std::string> resolve_test_src(const xml_loader& from, const pugi::xml_node& node){
        std::this_thread::sleep_for(std::chrono::seconds(rand()%10));
        return std::string("fake content");
    }

    static std::optional<std::string> resolve_test_content(const xml_loader& from, const pugi::xml_node& node){
        //TODO: I need to pass some fast information if auth_token or session_token is to be used.
        auto src = node.attribute("src").as_string();
        std::vector<uint8_t> w(src,src+strlen(src));

        cache.remote_content.get(w, from.auth_token);
        std::this_thread::sleep_for(std::chrono::seconds(rand()%15));
        return std::string("fake content");
    }

    //TODO: Add tcc

    void resolver_pass(const pugi::xml_node& node){

        const auto& src =node.attribute("src");
        if(!src.empty() && node.attribute("src.no-prefetch").empty()){
            cache.collect_entries.log(res_logger_t::NEEDED, node.path(), "Request collection for `%s`", src.as_string());
            pool.push_back(std::thread(&xml_loader::compile_artifact<std::string,false>, this, node, std::ref(cache.collect_entries), resolve_test_content));
        }
        
        if(strcmp(node.name(),"script")==0  && node.attribute("body.no-prefetch").empty()){
            cache.collect_entries.log(res_logger_t::NEEDED, node.path(), "Request compilation");
            pool.push_back(std::thread(&xml_loader::compile_artifact<std::string, true>, this, node, std::ref(cache.collect_entries), resolve_test_content));
        }
        
        for(auto& child: node.children()){
            resolver_pass(child);
        }
    }

    void build_pass(const pugi::xml_node& node){
        std::unique_lock lm(xml_tree_mutex);

        const auto& src =node.attribute("src");
        if(src.empty()){}
        else{
            cache.collect_entries.log(res_logger_t::WAIT_FOR, node.path(), "Waiting for collection of `%s`",src.as_string());

            if(node.attribute("src.no-prefetch")){
                cache.collect_entries.log(res_logger_t::NEEDED, node.path(), "In-place collection of `%s`", src.as_string());
                pool.push_back(std::thread(&xml_loader::compile_artifact<std::string,false>, this,node, std::ref(cache.collect_entries), resolve_test_content));
            }

            cv.wait(lm,[&node]{
                return !node.attribute("body.success").empty();
            });

            //TODO: fetching it from the cache verify its name signature to match. If not, invalidate the current cache and collect it again.
            /*
                auto tmp = cache.get({...})
                if(tmp->name!=name){
                    //TODO: Debug message to notify there was a collision
                    cache.invalidate({....})
                    auto data = resolve_....(this, ....)
                }
            */
 
            if(node.attribute("body.success").as_bool())cache.collect_entries.log(res_logger_t::PRODUCED, node.path(), "Resolved request for `%s`",src.as_string());  
            else cache.collect_entries.log(res_logger_t::FAILED, node.path(), "Failed request for `%s`",src.as_string());         
        }
        
        if(strcmp(node.name(),"script")==0){
            cache.collect_entries.log(res_logger_t::WAIT_FOR, node.path(), "Waiting for compilation");

            if(node.attribute("src.no-prefetch")){
                cache.collect_entries.log(res_logger_t::NEEDED, node.path(), "In-place compilation");
                pool.push_back(std::thread(&xml_loader::compile_artifact<std::string,false>, this,node, std::ref(cache.collect_entries),resolve_test_content));
            }

            cv.wait(lm,[&node]{
                return !node.attribute("body.success").empty();
            });
            if(node.attribute("body.success").as_bool())cache.collect_entries.log(res_logger_t::PRODUCED, node.path(), "Resolved compilation");
            else cache.collect_entries.log(res_logger_t::FAILED, node.path(), "Failed compilation");
        }
        
        for(auto& child: node.children()){
            lm.unlock();
            build_pass(child);
            lm.lock();
        }
        lm.unlock();
    }

    public:


    xml_loader(const char* filename, const xml_loader* parent=nullptr, std::optional<uuid> forced_scope= {}){
        pugi::xml_parse_result result = doc.load_file(filename);
        if (!result){
            throw "Bad file";
        }
    
        const auto& xml_root = doc.child("app");
        if(xml_root.empty()){
            throw "Not an app";
        }

        const auto& token = xml_root.attribute("auth.token");

        if(parent!=nullptr){
            this->depth = parent->depth+1;
            this->auth_token = parent->auth_token;
            this->auth_session = parent->auth_session;

            //Apply the forced scope
            if(forced_scope.has_value()){
                this->auth_token.mix(forced_scope.value());
                this->auth_session.mix(forced_scope.value());
            }

            if(!token.empty()){
                uuid new_base = token.as_string();
                this->auth_token.mix(new_base);
                this->auth_session.mix(new_base);
            }
        }
        else{
            if(forced_scope.has_value()){
                this->auth_token=forced_scope.value();
                this->auth_token.mix(token.as_string());
            }
            else{
                //VERY UNSAFE! We are using a clear key without crypto safety.
                this->auth_token=token.as_string();
            }
            this->auth_session.set_random();
        }

    }

    int run(){
        //TODO:Check for scrit.global, handle its src and body first, tag it, so that the next worker can skip it.
        resolver_pass(doc);
        build_pass(doc);


        for(auto& i : pool){
            i.join();
        }
        pool.clear();

        return 0;
    }

};



int main(){
    std::string ww("hello\0world",7), ww2=ww;
    std::string ww3("hello\0w1rld",7);
    
    std::cout<<"..........."<<ww2[6]<<"   "<<(ww==ww3)<<true<<"\n";
    xml_loader hello("./examples/parallel-loader.xml");
    std::cout<<"\n--------------------------------------\n";
    hello.run();
    xml_loader hello2("./examples/parallel-loader.xml");
    std::cout<<"\n--------------------------------------\n";
    hello.run();
    std::cout<<"\n--------------------------------------\n";
}
