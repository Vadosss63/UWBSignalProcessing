#ifndef PTREECOMMANDS_H
#define PTREECOMMANDS_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

class PTreeCommands
{
public:
    PTreeCommands();
    ~PTreeCommands();
private:
    boost::property_tree::ptree m_ptreeJson;
};

#endif // PTREECOMMANDS_H
