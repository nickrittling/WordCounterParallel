
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <CL/sycl.hpp>


class BloomFilter {
public:
  BloomFilter(long long int m, int k, std::string s)
      : m_(m), k_(k), file(s), dict(m, 0), conflict(0), n_(0) {
    std::ifstream infile;
    infile.open(file);
    if (!infile)
      std::cout << "File not found\n";
    std::string line;
    std::vector<std::string> temp;
    while (getline(infile, line)) {
      n_++;
      temp.push_back(line);
      //insert(line);
    }
    infile.close();

    sycl::queue q;
    sycl::buffer<char,1> buf(temp, sycl::range<1>(temp.size())); 
    q.submit([&](sycl::handler& h){
		    auto data_acc = buf.get_access<sycl::access::mode::read_write>(h);
		    h.parallel_for<temp.size(), [=](id<1> i){
		   	insert(temp[i]);
			});//end submit

    });    



  }//end constructor

  int getConfilct() { return conflict; }

  size_t hash1(std::string s) { return std::hash<std::string>{}(s) % m_; }

  size_t hash2(std::string s) {
    unsigned char sha256_result[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)s.c_str(), s.length(), sha256_result);
    size_t result;
    std::memcpy(&result, sha256_result, sizeof(size_t));
    return result % m_;
  }
  size_t hash3(std::string s) {
    unsigned char md5_result[MD5_DIGEST_LENGTH];
    MD5((unsigned char *)s.c_str(), s.length(), md5_result);
    size_t result;
    std::memcpy(&result, md5_result, sizeof(size_t));
    return result % m_;
  }

  void insert(std::string s) {
	  std::string mname = "";
    if (k_ == 2) {
      if (dict[hash1(s)]) {
        conflict++;
      }

      dict[hash1(s)] = 1;
      mname += std::to_string(hash1(s));
      if (dict[hash2(s)]) {
        conflict++;
      }

      dict[hash2(s)] = 1;
      mname += std::to_string(hash2(s));
      WC[mname] = 0;


    } // end k 2
    else {
      if (dict[hash1(s)]) {
        conflict++;
      }

      dict[hash1(s)] = 1;
      mname += std::to_string(hash1(s));

      if (dict[hash2(s)]) {
        conflict;
      }

      dict[hash2(s)] = 1;
      mname += std::to_string(hash2(s));

      if (dict[hash3(s)]) {
        conflict++;
      }

      dict[hash3(s)] = 1;
      mname += std::to_string(hash3(s));
      WC[mname] = 0;
    }
  } // end insert
  

  void wordcount(std::string s) {
          std::string mname = "";
    if (k_ == 2) {
      mname += std::to_string(hash1(s));
      mname += std::to_string(hash2(s));

      WC[mname]++;


    } // end k 2
    else {
      mname += std::to_string(hash1(s));
      mname += std::to_string(hash2(s));
      mname += std::to_string(hash3(s));
      WC[mname]++;
    }
  } // end wordcount


  void WCStart(){
	std::ifstream infile;
    	infile.open(file);
    	if (!infile)
      	std::cout << "File not found\n";
    	std::string line;
	std::vector<std::string> temp;
    	while (getline(infile, line)) {
        	temp.push_back(line);
   	 }
   	infile.close();

	sycl::queue q;
    	sycl::buffer<char,1> buf(temp, sycl::range<1>(temp.size()));
    	q.submit([&](sycl::handler& h){
                    auto data_acc = buf.get_access<sycl::access::mode::read_write>(h);
                    h.parallel_for<temp.size(), [=](id<1> i){
                        wordcount(temp[i]);
                        });//end submit

    });


  }//end WCStart


  void output() {
    double prob = std::pow((1 - std::pow((1 - (1 / m_)), k_ * n_)), k_);
    std::cout << "m:" << m_ << " k:" << k_ << " n:" << n_ << std::endl;
    std::cout << "false positive probability: " << prob << std::endl;
    std::cout << "number of conflict: " << conflict << std::endl;
  }//end output

  void printMap(){
	  for (const auto& pair : WC) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
  }//end printMap

  void outputMap(){
	  std::ofstream of;
  of.open("WordCountOut.txt", std::fstream::out);
  if (!of)
    std::cout << "File WordCountOut not found\n";

  for(const auto& pair : WC){
	  of << pair.first << ": " << pair.second << std::endl;
  }
  }//end outputMap()

private:
  long long int m_;
  int k_;
  int n_;
  int conflict;
  std::string file;
  std::vector<bool> dict;
  std::map<std::string, int> WC;
}; // end class BloomFIlter
