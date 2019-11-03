#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include <iostream>
#include <typeinfo>

namespace ptree = boost::property_tree;

int main() {
    ptree::ptree jsonpt;
    // READING JSON FILE
    ptree::read_json("ptree.json", jsonpt);
    int cnt = 1;
    for (auto& rc : jsonpt.get_child("menu")) {
        std::cout << rc.first << " : " << rc.second.data() << "\n";
        if (rc.second.get<std::string>("") == "")
            for (auto& crc : rc.second.get_child(""))
                for (auto& ccrc : crc.second.get_child(""))
                std::cout << "\t" << ccrc.first << " : " << ccrc.second.data() << "\n";
    }


    // WRITING TO XML FILE
    ptree::ptree xmlpt;
    xmlpt.put("menu.drinks.beverage", "coke");
    xmlpt.add("menu.drinks.alcohoolic", "wine");
    xmlpt.add("menu.drinks.beverage", "water");
    xmlpt.put("menu.food.fish", "salmon");
    xmlpt.add("menu.food.fish", "saint-peter");
    xmlpt.put("menu.food.meat", "prime rib");
    xmlpt.add("menu.food.meat", "T-bone");

    ptree::xml_writer_settings<std::string> settings(' ', 4);

    ptree::write_xml("ptree.xml", xmlpt, std::locale() , settings);

}