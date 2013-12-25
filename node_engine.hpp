/**
 * This is a factory that takes an XML document and creates a tree
 * of XML_Nodes from it. It also provides a signal to notify
 * listeners of new nodes, but keep in mind that the branch
 * nodes will be available prior to the nodes above them. The best
 * strategy, if you're going to hook into this signal, would be
 * to listen for a specific node that describes a chunk of what
 * you're looking for and then iterate down into that node's children
 * to get any additional data you're looking for.
 *
 * Copyright 2013 Bruce Ide
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef _HPP_NODE_ENGINE
#define _HPP_NODE_ENGINE

#include <boost/bind.hpp>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include "xml_node.hpp"

namespace cppxml {
  
  typedef std::pair<std::string, std::string> namespace_pair;
  typedef std::vector<namespace_pair> namespace_vector;

  
  class node_engine {

    engine expat_engine;
    child_vector node_stack;
    node_pointer document;
    node_pointer current_node;
    namespace_vector namespaces;

    /**
     * The nice thing about engine using boost signals is that I can
     * actually bind this to its signals, so I don't have to play
     * the game I did in that class of using pointers to static
     * functions in my class.
     */

    void setup_listeners()
    {
      expat_engine.start.connect(boost::bind(&node_engine::handle_start, this, _1, _2));
      expat_engine.end.connect(boost::bind(&node_engine::handle_end, this, _1));
      expat_engine.text.connect(boost::bind(&node_engine::handle_text, this, _1));
      expat_engine.namespace_start.connect(boost::bind(&node_engine::handle_namespace_start, this, _1, _2));
      expat_engine.namespace_end.connect(boost::bind(&node_engine::handle_namespace_end, this, _1));
    }

    void handle_start(const std::string &name, const attribute_vector &attributes) 
    {
      if (current_node.get() == nullptr) {
	current_node = std::make_shared<xml_node>();
	document = current_node;
      } else {
	node_stack.push_back(current_node);
	current_node = std::make_shared<xml_node>();
      }
      std::string node_name, node_ns(""), node_ns_uri("");
      if (namespaces.size() > 0) {
	for (namespace_pair ns : namespaces) {
	  if (name.find(ns.second) != std::string::npos) {
	    node_ns = ns.first;
	    node_ns_uri = ns.second;
	    node_name = name.substr(node_ns_uri.length() + 1);
	    break;
	  }
	}
      } else {
	node_name = name;
      }
      current_node->set_name(node_name);
      current_node->set_namespace(node_ns);
      current_node->set_namespace_uri(node_ns_uri);
      for (attribute_pair attribute : attributes) {
	current_node->set_attribute(attribute.first, attribute.second);
      }
    }

    void handle_end(const std::string &name)
    {
      // If the XML is mal-formed, engine will complain about it
      // in get_document. So we can pretty much just ignore name.
      
      notify(current_node);
      node_pointer last_node = current_node;
      if (node_stack.size() > 0) {
	current_node = node_stack.back();
	node_stack.pop_back();
	current_node->add_child(last_node);
      } // Otherwise that's it, document's done.
    }

    void handle_text(const std::string &text)
    {
      current_node->set_text(text);
    }

    void handle_namespace_start(const std::string &ns, const std::string &ns_uri)
    {
      namespaces.push_back(make_pair(ns, ns_uri));
    }

    void handle_namespace_end(const std::string &ns)
    {
      for (namespace_vector::iterator i = namespaces.begin(); i != namespaces.end() ; ++i) {
	if (i->first == ns) {
	  namespaces.erase(i);
	  break; // I assume there can only be one...
	}
      }
    }

  public:

    node_engine(std::string filename) throw(std::string) : expat_engine(filename)
    {
      setup_listeners();
    }

    node_engine(std::istream &xml_stream) throw(std::string) : expat_engine(xml_stream)
    {
      setup_listeners();
    }

    ~node_engine()
    {
      document.reset();
    }

    node_pointer get_document() throw(std::string)
    {
      if (document.get() == nullptr) {
	expat_engine.parse();
      }
      if (document.get() == nullptr) {
	throw std::string(nullptr);
      }
      return document;
    }

    boost::signals2::signal<void(node_pointer)> notify;
    
  };
  
}

#endif
