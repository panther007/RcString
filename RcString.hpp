/*
 * This is a reference counted string implementation.  Two RcString objects
 * that have a common ancestor and contain the original string should point to
 * the same data object.  This provides very fast copies.
 *
 * You are not allowed to use std::string to hold the string contents.
 */

#define DEBUG_STM(fmt) \
        std::cout<<__FILE__<<":"<<__FUNCTION__<<":"<<__LINE__<<": "<<fmt<<std::endl

#ifndef CS540_RC_STRING_HPP
#define CS540_RC_STRING_HPP

#include<cstring>
#include<iostream>

namespace cs540 {


  class RcString {
    private:
      class RcString_data {
        friend class RcString;
        int ref_count;
        char* str;
        public:
          RcString_data() : ref_count(1), str(new char[1]){
            std::strcpy(str, "");
          }
          RcString_data(const char* c) : ref_count(1), str(new char[strlen(c)+1]){
            std::strcpy(str, c);
          }
          virtual ~RcString_data(){
            delete [] str;
          }
          
          friend std::ostream& operator<<(std::ostream& os, const RcString& r);
          friend RcString operator+(const RcString& r1, const RcString& r2);
          friend RcString operator+(const RcString& r, const char* c);
      };
        
        class RcString_data_helper : RcString_data{
          friend class RcString;
          public:
            RcString_data_helper() : RcString_data(){
            }
            virtual ~RcString_data_helper(){
            }
        };
    public:
        // Public methods.
        RcString() : m_data(new RcString_data){
	      }
        
        RcString(const char* c) : m_data(new RcString_data(c)) {
	      }
        
        RcString(std::string&& s) : m_data(new RcString_data(s.c_str())) {
        //RcString(std::string s) : m_data(new RcString_data(s.c_str())) {
        //RcString(std::basic_string<char>&& s) : m_data(new RcString_data(s.c_str())) {
	      }
        
        RcString(const RcString& r) : m_data(r.m_data){
          m_data->ref_count++;
	      }
        
        RcString(RcString&& r) : m_data(std::move(r.m_data)){
	      }
        
        ~RcString() {
           // Delete the data object if the ref count goes to 0.
           if (--m_data->ref_count == 0) {
             delete m_data;
           }
        }
        
        RcString& operator=(const RcString& r){
          if(this == &r) return *this;
          if (--m_data->ref_count == 0) {
             delete this->m_data;;
          }
          this->m_data = r.m_data;
          r.m_data->ref_count++;
          return *this;
        }
        
        RcString& operator=(const RcString&& r){
          if (--m_data->ref_count == 0) {
             delete this->m_data;
          }
          this->m_data = std::move(r.m_data);
          r.m_data->ref_count++;
          return *this;
        }
   
        friend std::ostream& operator<<(std::ostream& os, const RcString& r){
          os<<r.m_data->str;
	        return os;
	      }

        friend RcString operator+(const RcString& r1, const RcString& r2){
          RcString Temp;
          delete [] Temp.m_data->str;
          Temp.m_data->str = new char[strlen(r1.m_data->str) + 1 + strlen(r2.m_data->str)];
          std::strcpy(Temp.m_data->str, r1.m_data->str);
          std::strcat(Temp.m_data->str, r2.m_data->str);
          return Temp;
        }

        friend RcString operator+(const RcString& r, const char* c){
          RcString Temp;
          delete [] Temp.m_data->str;
          Temp.m_data->str = new char[strlen(r.m_data->str) + 1 + strlen(c)];
          std::strcpy(Temp.m_data->str, r.m_data->str);
          std::strcat(Temp.m_data->str, c);
          return Temp;
        }
        
    private:
        RcString_data *m_data;
};



}



#endif
