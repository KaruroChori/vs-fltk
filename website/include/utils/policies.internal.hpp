#pragma once
#include "policies.hpp"
#include <pugixml.hpp>

namespace vs{

namespace internal{

struct policies_t : vs::policies_t{
    void inherit_from_xml(const pugi::xml_node& root);
    void save_to_xml(pugi::xml_node root);
};

}
}