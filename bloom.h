
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <CL/sycl.hpp>


class BloomFilter {
public:
  BloomFilter(long long int m, std::string s)
      : m_(m), file(s), dict(m, 0), conflict(0), n_(0) {
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


  void insert(std::string s) {
      std::string mname = "";

      dict[hash1(s)] = 1;
      mname += std::to_string(hash1(s));
      WC[mname] = 0;
  } // end insert
  

  void wordcount(std::string s) {
      std::string mname = "";
      mname += std::to_string(hash1(s));
      WC[mname]++;
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
  int n_;
  int conflict;
  std::string file;
  std::vector<bool> dict;
  std::map<std::string, int> WC;
}; // end class BloomFIlter
