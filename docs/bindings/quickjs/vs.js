//TODO: Move out from quicjs-js.cpp into here! Without #embed it is not that easy :(.
var $$ = (string, base = this.core.self) => {
    let self = (string === undefined ? base : this.core.resolve_name_path(base, string));
    return {
        __self: self,
        $: (string) => $$(string, self),
        prop: (key, value) => this.core.apply_prop(self, key, value),
        computed: (key) => this.core.get_computed(self, key),
        mixin: (value) => this.core.apply_prop(self, "mixin", value),
        call: (name, ...args) => this.core.call(self, name, ...args),
        log: (sev, string, ...args) => this.core.log(sev, self, string, ...args),
    }
};

//TODO: decide what to do with this and which interface is best.
var $$2 = (string, base = this.core.self) => {
    let self = (string === undefined ? base : this.core.resolve_name_path(base, string));
    return {
        __self: self,
        $: (string) => $$2(string, self),
        prop: new Proxy(this,{
            get(target,prop,receiver){
                globalThis.core.get_computed(target.__self, prop);
            },
            set(target,prop,value,receiver){
                globalThis.core.apply_prop(target.__self, prop, value);
            }
        }),
        state: new Proxy(this,{
            get(target,prop,receiver){
                globalThis.core.getter(target.__self, prop);
            },
            set(target,prop,value,receiver){
                globalThis.core.setter(target.__self, prop, value);
            }
        }),
        mixin: (value) => this.core.apply_prop(self, "mixin", value),
        call: (name, ...args) => this.core.call(self, name, ...args),
        log: (sev, string, ...args) => this.core.log(sev, self, string, ...args),
    }
};

var LOG = Object.freeze({
    INFO: 0,
    OK: 1,
    WARNING: 2,
    CONTINUE: 3,
    PANIC: 4,
    LOG: 5,
    LVL_SILENT: 0x00,
    LVL_NORMAL: 0x10,
    LVL_VERBOSE: 0x20,
    LVL_DEBUG: 0x40,
});

var $ = $$();

var $cb = (fn) => { globalThis[`__EXPORT_CB__${fn.name}`] = fn; };
var $callback = $cb;
var $plotter = (fn) => { globalThis[`__EXPORT_DRW_${fn.name}`] = fn; };
var $getter = (fn) => { globalThis[`__EXPORT_GET_${fn.name}`] = fn; };
var $setter = (fn) => { globalThis[`__EXPORT_SET_${fn.name}`] = fn; };
var $fn = (fn) => { globalThis[`__EXPORT_UKN_${fn.name}`] = fn; };


/**
 * Set widget:prt_t to vs_self and return the old value;
 */
function vs_set_env(widget){
    const t = globalThis.core.self;
    globalThis.core.self=widget;
    return t;
}