/**
 * A basic node that can be used to build a document tree. This is not
 * meant to be anything like DOM. If anything, I think it's somewhat
 * close to Ruby's rexml.
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

#ifndef _CPPXML_HPP_XML_NODE
#define _CPPXML_HPP_XML_NODE

#include "engine.hpp"
#include <string>
#include <memory>
#include <vector>

namespace cppxml {

  class xml_node; // pre-declare yadda

  typedef std::shared_ptr<xml_node> node_pointer;
  typedef std::vector<node_pointer> child_vector;

  class xml_node {
  private:

    attribute_vector attributes;
    std::string name;
    std::string ns;
    std::string ns_uri;
    std::string text;
    child_vector children;

    // Private version allows me to pass a reference to the vector around
    // rather than copying a lot. Also: Yay! Recursion!

    void find_children(const std::string &name, child_vector &children)
    {
      for(node_pointer child : this->children) {
	if (child->name == name) {
	  children.push_back(child);
	}
	if (child->children.size() > 0) {
	  child->find_children(name, children);
	}
      }
    }

  public:

    xml_node() : name(""), ns(""), ns_uri(""), text("")
    {
    }

    xml_node(const std::string &name, const std::string &ns, const std::string ns_uri) : name(name), ns(ns), ns_uri(ns_uri), text("")
    {
    }

    xml_node(const xml_node &tocopy) : attributes(tocopy.attributes), name(tocopy.name), ns(tocopy.ns), ns_uri(tocopy.ns_uri), text(tocopy.text)
    {
      for (node_pointer child : children) {
	child.reset();
      }
    }

    ~xml_node()
    {
    }

    void add_child(const node_pointer &child)
    {
      children.push_back(child);
    }

    std::string get_name()
    {
      return name;
    }

    void set_name(const std::string &name)
    {
      this->name = name;
    }

    std::string get_text()
    {
      return text;
    }

    void set_text(const std::string &text)
    {
      this->text.append(text);
    }

    std::string get_namespace()
    {
      return ns;
    }

    void set_namespace(const std::string &ns)
    {
      this->ns = ns;
    }

    std::string get_namespace_uri()
    {
      return ns_uri;
    }

    void set_namespace_uri(const std::string &ns_uri)
    {
      this->ns_uri = ns_uri;
    }

    void set_attribute(std::string key, std::string value)
    {
      attributes.push_back(make_pair(key, value));
    }

    /**
     * Finds an attribute key and returns a const pointer to its value,
     * or nullptr if the attribute is not found.
     */

    const std::shared_ptr<std::string>find_attribute(const std::string &key)
    {
      std::shared_ptr<std::string> retval;
      for (attribute_pair attribute : attributes) {
	if (attribute.first == key) {
	  retval = std::make_shared<std::string>(attribute.second);
	  break;
	}
      }
      return retval;
    }
    
    /**
     * Returns vector of all attributes
     */

    attribute_vector all_attributes()
    {
      return attributes;
    }

    /**
     * Attempts to find the first child node with a given tag name.
     * Returns that if found or an empty shared_ptr if no child is
     * found.
     */

    node_pointer find_child(const std::string &name)
    {
      node_pointer retval;
      for (node_pointer child : children) {
	if (child->name == name) {
	  retval = child;
	  break;
	}
	node_pointer child_search = child->find_child(name);
	if (child_search.get() != nullptr) {
	  retval = child_search;
	  break;
	}
      }
      return retval;
    }

    /**
     * Finds all children and returns them in a vector.
     */

    child_vector find_children(const std::string &name)
    {
      child_vector children;
      find_children(name, children);
      return children;
    }

    /**
     * All (immediate) children
     */

    child_vector all_children()
    {
      return children;
    }    

  };

}

#endif
