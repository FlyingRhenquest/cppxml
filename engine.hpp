/**
 * A basic C++ wrapper for expat's SAX parser.
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

#ifndef _CPPXML_HPP_ENGINE
#define _CPPXML_HPP_ENGINE

#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <expat.h>
#include <fstream>
#include <iostream>
#include <utility>

namespace cppxml {

  typedef std::pair<std::string, std::string> attribute_pair;
  typedef std::vector<attribute_pair> attribute_vector;


  class engine {
    bool close_doc;
    std::istream *doc;
    size_t expat_buffer_len;
    char *expat_buffer;
    XML_Parser expat_parser;

    // Creates a parser and registers this object's callbacks

    void setup_expat() throw (std::string)
    {
      expat_buffer = nullptr;
      set_expat_buffer_len();
      expat_parser = XML_ParserCreateNS((XML_Char *) NULL, ':');
      if (NULL == expat_parser) {
	throw std::string("Error allocating expat parser.");
      }
      // Set this as our data pointer, so we can call it from
      // our various static handlers later on.
      XML_SetUserData(expat_parser, (void *) this);
      XML_SetElementHandler(expat_parser,
			    _handle_start,
			    _handle_end);
      XML_SetCharacterDataHandler(expat_parser, _handle_text);
      XML_SetNamespaceDeclHandler(expat_parser, _handle_namespace_start, _handle_namespace_end);

    }

    // Since we're calling to a pointer to a function that isn't
    // object oriented, we need to make a function expat can call.
    // A static function doesn't have an implicit this, so we can
    // set expat to interface with the object this way. (Would be
    // nice if we could just boost::bind it, but whatever...)

    static void _handle_start(void *expat_this, const char *el, const char **attributes)
    {
      static_cast<engine *>(expat_this)->handle_start(el, attributes);
    }
  
    static void _handle_end(void *expat_this, const char *el) {
      static_cast<engine *>(expat_this)->handle_end(el);
    }

    static void _handle_text(void *expat_this, const char *text, int len)
    {
      std::string textstr(text, len);
      static_cast<engine *>(expat_this)->handle_text(textstr);
    }

    static void _handle_namespace_start(void *expat_this, const char *prefix, const char *uri)
    {
      static_cast<engine *>(expat_this)->handle_namespace_start(prefix, uri);
    }

    static void _handle_namespace_end(void *expat_this, const char *prefix)
    {
      static_cast<engine *>(expat_this)->handle_namespace_end(prefix);
    }

    // Check to see if we're done reading the stream. Modifies
    // done appropriately.
    void check_done(int &done, size_t &bytes_read)
    {
      bytes_read = doc->gcount();

      if (bytes_read < expat_buffer_len || !doc->good()) {
	done = 1;
      }
    }

    void handle_start(const char *el, const char **attr)
    {
      attribute_vector attributes;
      std::string element_name(el);
      for(int i = 0; attr[i]; i+= 2) {
	attributes.push_back(std::make_pair(std::string(attr[i]), std::string(attr[i + 1])));
      }
      start(element_name, attributes);
    }

    void handle_end(const char *el)
    {
      std::string element_name(el);
      end(element_name);
    }

    void handle_text(const std::string &textstr)
    {
      text(textstr);
    }

    void handle_namespace_start(const char *prefix, const char *uri)
    {
      std::string uri_prefix(prefix);
      std::string uri_uri(uri);
      namespace_start(uri_prefix, uri_uri);
    }

    void handle_namespace_end(const char *prefix)
    {
      std::string uri_prefix(prefix);
      namespace_end(uri_prefix);
    }

  public:


    /**
     * Construct an engine with a file name.
     */
    engine(std::string filename) throw(std::string) : close_doc(true), doc(new std::fstream(filename, std::fstream::in))
    {
      if (!doc->good()) {
	delete doc;
	throw std::string("Unable to open XML document");
      }
      setup_expat();
    }

    /**
     * Or an istream (Which could be a strstr or something.) If you
     * construct one this way, you'll have to close your istream
     * yourself when you're done with it.
     */

    engine(std::istream &xml_stream) : close_doc(false), doc(&xml_stream)
    {
      setup_expat();
    }

    ~engine()
    {
      XML_ParserFree(expat_parser);
      if (expat_buffer != nullptr) {
	delete expat_buffer;
      }
      if (close_doc) {
	dynamic_cast<std::fstream *>(doc)->close();
	delete doc;
      }
    }

    /**
     * Set expat buffer length if you want it to be soemething other
     * than the default
     */

    void set_expat_buffer_len(size_t len = 8096)
    {
      expat_buffer_len = len;
      if (expat_buffer != nullptr) {
	delete expat_buffer;
      }
      expat_buffer = new char[len];
      memset(expat_buffer, '\0', expat_buffer_len);
    }

    /**
     * Runs the expat parser. If it craps out with an error, throw the
     * error string.
     */

    void parse() throw(std::string)
    {
      int done = 0;
      size_t bytes_read;
      while (!done) {
	doc->read(expat_buffer, expat_buffer_len);
	check_done(done, bytes_read);
	if (XML_Parse(expat_parser, expat_buffer, bytes_read, done) == XML_STATUS_ERROR) {
	  std::stringstream err;
	  err << XML_ErrorString(XML_GetErrorCode(expat_parser)) << " at line " << XML_GetCurrentLineNumber(expat_parser);
	  throw err.str();	  
	}	
      }
    }

    boost::signals2::signal<void(const std::string &, const attribute_vector &)> start;
    boost::signals2::signal<void(const std::string &)> end;
    boost::signals2::signal<void(const std::string &)> text;

    /**
     * Note that if your XML actually uses namespaces, expat will
     * expand all tags to include the namespace URI. If you don't
     * actually want that, you'll have to adjust your tags
     * later on (See the test_namespace_handling test in engine_test.cpp
     * in the test directory for an example of how your tags will look.)
     */

    boost::signals2::signal<void(const std::string &, const std::string &)> namespace_start;
    boost::signals2::signal<void(const std::string &)> namespace_end;
  };
  
}

#endif
