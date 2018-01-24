/**
 * Implementations for the SimpleHashNode class.
 * For more info @see "SimpleHashNode.h".
 */
#include "SimpleHashNode.h"

#include <ctime>
#include <string>
#include <sstream>
#include "PicoSHA2/picosha2.h"

/**
 * Construct a new SimpleHashNode given the previous node in the chain and the payload
 * to include with the new node.
 * 
 * @param prev_node Previous node in the hash chain.
 * @param payload   Payload (string) to be included in this node.
 */
SimpleHashNode::SimpleHashNode(const SimpleHashNode& prev_node, const std::string& payload)
: serial{prev_node.serial + 1},
  timestamp{time(0)},
  prev_hash{prev_node.get_hash()},
  payload{payload}
{}

/**
 * Construct a new SimpleNode given a character-encoded input stream.
 * @details If the stream does not contain the correct values to create the node, 
 * a default, uninitialized node is returned and the stream's `fail` bit is set.
 * 
 * @param strm Stream from which the new node will be extracted.
 */
SimpleHashNode::SimpleHashNode(std::istream& strm){
    std::string buffer;
    char sep;
    
    strm >> serial >> sep;
    if(sep != field_sep)
        strm.setstate(std::ios::failbit);
    
    strm >> timestamp >> sep;
    if(sep != field_sep)
        strm.setstate(std::ios::failbit);
    
    getline(strm, buffer, field_sep);
    try{
        prev_hash = hash_str_to_raw(buffer);
    }
    catch(...){
        strm.setstate(std::ios::failbit);
    }
    getline(strm, buffer, field_sep);
    payload   = buffer;
    
    raw_hash   hash;
    getline(strm, buffer);
    try{
        hash = hash_str_to_raw(buffer);
    }
    catch(...){
        strm.setstate(std::ios::failbit);
    }
    
    if(strm.fail() || strm.bad() || hash != get_hash()){        // Check validity of the input
        *this = SimpleHashNode{};  // Set to uninitialized node if input doesn't validate.
    }
}

/**
 * Returns `true` if the current node has not been initialized as either a genesis node or
 * a true node in a hash chain.
 * 
 * @return `true` if the current node has not been initialized.
 */
bool SimpleHashNode::is_uninitialized() const{
    return timestamp == 0u || (serial == 0u && payload != std::string{"Genesis Node"});
}
  
/**
 * Returns the node's serial number.
 * @return the current node's serial number.
 */
serial_t SimpleHashNode::get_serial() const{
    return serial;
}

/**
 * Returns the node's timestamp as a UNIX-style timestamp.
 * @return the current node's timestamp
 */
time_t SimpleHashNode::get_timestamp() const{
    return timestamp;
}

/**
 * Returns the node's payload.
 * @return the current node's payload (as a std::string).
 */
std::string SimpleHashNode::get_payload() const{
    return payload;
}

/**
 * Get a raw byte array version of the previous node's hash.
 * @return a byte array of the previousnode's hash.
 */
raw_hash_t SimpleHashNode::get_prev_hash() const{
    return static_cast<raw_hash_t>(prev_hash);
}

/**
 * Get a raw byte array version of the current node's hash.
 * @return a byte array of the node's hash.
 */
raw_hash_t SimpleHashNode::get_hash() const{
    return static_cast<raw_hash_t>(compute_hash(*this));
}

/**
 * Get a hexadecimal string version of the previous node's hash.
 * @return a hexadecimal string of the previous node's hash.
 */
std::string SimpleHashNode::get_prev_hash_str() const{
    return picosha2::bytes_to_hex_string(prev_hash);
}

/**
 * Get a hexadecimal string version of the current node's hash.
 * @return a hexadecimal string of the node's hash.
 */
std::string SimpleHashNode::get_hash_str() const{
    return picosha2::bytes_to_hex_string(get_hash());
}

/**
 * Returns a string containing a human-friendly representation of 
 * the node and its payload.
 * 
 * @details Use the `verbose` flag to also include the hashes of 
 * the current and previous nodes.
 * 
 * @param verbose flag indicating hash info should be included.
 * @return a std::string representation of the node
 */
std::string SimpleHashNode::info(bool verbose) const{
    std::ostringstream oss;
    oss << "Serial:     " << serial    << (verbose ? '\n' : '\t');
    oss << "Timestamp : " << timestamp << '\n';
    if(verbose){
        oss << "Prev Hash : " << get_prev_hash_str() << '\n';
    }
    oss << "Payload:\n";
    oss << payload;
    oss << '\n';
    if(verbose){
        oss << "This Hash : " << get_hash_str() << '\n';
    }
    return oss.str();
}

/**
 * Factory function for producing a genesis node for starting a 
 * new hash chain.
 * 
 * @details The genesis node's timestamp will be the time of its
 * construction, the `prev_hash` will be all zeros, and the payload
 * is the literal string "Genesis Node".
 * 
 * @return a new genesis node
 */
SimpleHashNode SimpleHashNode::create_genesis_node(){
    auto genesis = SimpleHashNode{};
    genesis.timestamp = time(0);
    genesis.payload   = std::string{"Genesis Node"};
    return genesis;
}

/**
 * Computes hash of `node` and returns it as raw bytes.
 * 
 * @param node node whose hash will be computed
 * @return raw bytes representing the hash of `node`
 */
raw_hash   SimpleHashNode::compute_hash(const SimpleHashNode& node){
    picosha2::hash256_one_by_one hasher;
    hasher.process(&node.serial, &node.serial       + sizeof(node.serial));
    hasher.process(&node.timestamp, &node.timestamp + sizeof(node.timestamp));
    hasher.process(node.prev_hash.begin(), node.prev_hash.end());
    hasher.process(node.payload.begin(),   node.payload.end());
    hasher.finish();
    raw_hash   hash;
    hasher.get_hash_bytes(hash.begin(), hash.end());
    return hash;
}

/**
 * Write a node in an efficient format into a character-encoded stream.
 * 
 * @param strm stream to write the node into
 * @param node the node to output
 */
std::ostream& operator << (std::ostream& strm, const SimpleHashNode& node){
    auto sep = SimpleHashNode::field_sep;
    strm << node.get_serial()        << sep 
         << node.get_timestamp()     << sep
         << node.get_prev_hash_str() << sep
         << node.get_payload()       << sep
         << node.get_hash_str();
    return strm;
}

/**
 * Read a node from a character stream with stream extraction.
 * If the extraction fails, the node will be left in the `is_uninitialized` state.
 * @param strm stream to read from
 * @param node destination node
 */
std::istream& operator >> (std::istream& strm, SimpleHashNode& node){
    node = SimpleHashNode{strm};
    return strm;
}

/**
 * converts a hash in string form to raw bytes
 * 
 * @remarks Based on code from https://stackoverflow.com/a/30606613
 * 
 * @param hash_str      string form of the hash
 * @return              raw bytes representation of the hash
 */
raw_hash   SimpleHashNode::hash_str_to_raw(const std::string& hash_str) const{
    const auto hex_base        = 16;
    const auto hash_str_length = 64;
    raw_hash   bytes;
    
    if(hash_str.length() != hash_str_length){
        std::ostringstream err;
        err << "Unexpected hash string length. Expected 64 characters. ";
        err << "Received " << hash_str.length() << " characters.";
        throw std::runtime_error{err.str()};
    }

    for (size_t i = 0, b = 0; i < hash_str_length; i += 2, ++b) {
        auto byteString = hash_str.substr(i, 2);
        auto byte       = static_cast<unsigned char>(std::stol(byteString, 0, hex_base));
        bytes[b] = byte;
    }

    return bytes;
}