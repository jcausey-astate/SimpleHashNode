#include <iostream>
#include <sstream>
#include <list>
#include <fstream>
#include <thread>
#include <chrono>

#include "SimpleHashNode.h"

#include "PicoSHA2/picosha2.h"
    
int main() {
    std::list<SimpleHashNode> chain;
    auto genesis = SimpleHashNode::create_genesis_node();
    std::cout << genesis.info(true) << std::endl;
    chain.push_back(genesis);
    
    for(int i = 0; i < 10; ++i){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::ostringstream oss;
        oss << "Node # " << (i + 1);
        chain.push_back( SimpleHashNode(chain.back(), oss.str()) );
        std::cout << chain.back().info(true) << std::endl;
    }

    std::ofstream fout{"/tmp/hash_chain_data.txt"};
    bool first = true;
    for(const auto& node : chain){
        if(!first){
            fout << '\n';
        }
        fout << node;
        first = false;
    }
    fout.close();

    std::list<SimpleHashNode> chain2;
    std::ifstream fin{"/tmp/hash_chain_data.txt"};
    SimpleHashNode node;
    while(fin >> node){
        chain2.push_back(node);
    }
    std::cout << "\n\nRe-built from file:\n";
    
    raw_hash prev;
    for(const auto v : chain2){
        std::cout << v.info(true) << "\n";
        if(v.get_prev_hash() != prev){
            std::cout << "Hash check failed.\n" << std::endl;
        }
        prev = v.get_hash();
    }

    // Verify that the fin stream is 'fail'ed now:
    if( !(fin.fail() || fin.eof()) ){
        std::cout << "Error: The input stream should be failed or eof at this point." << std::endl;
    }

    return 0;
}
