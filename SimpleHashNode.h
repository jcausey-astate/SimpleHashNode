/**
 * SimpleHashNode is a node for use in building a hash chain (i.e. "Blockchain").
 * It is designed to be simple but flexible, by storing a variable-length string 
 * as payload.
 */
#ifndef SIMPLE_HASH_NODE_H
#define SIMPLE_HASH_NODE_H

#include <cstdint>
#include <array>
#include <string>

typedef uint64_t                      serial_t;
typedef std::array<unsigned char, 32> raw_hash_t;

/**
 * raw_hash is a convenience wrapper around the `raw_hash_t` 
 * typedef to always construct a raw hash with all zero bytes.
 */
class raw_hash : public raw_hash_t {
public:
    raw_hash()                    : raw_hash_t{ {0} } {}
    raw_hash(const raw_hash_t& h) : raw_hash_t{h} {};
    raw_hash(raw_hash_t&& h)      : raw_hash_t{h} {};
    raw_hash& operator= (const raw_hash_t& h){ 
        *this = raw_hash{h}; 
        return *this;
    }
};

/**
 * SimpleHashNode is a node for use in building a hash chain (i.e. "Blockchain").
 * It is designed to be simple but flexible, by storing a variable-length string 
 * as payload.
 */
class SimpleHashNode {
public:
    // Constructors
    SimpleHashNode(const SimpleHashNode& prev_node, const std::string& payload);
    SimpleHashNode(std::istream& strm);
    SimpleHashNode() = default;
    
    // Accessors
    bool        is_uninitialized()       const;
    serial_t    get_serial()             const;
    time_t      get_timestamp()          const;
    raw_hash_t  get_prev_hash()          const;
    std::string get_prev_hash_str()      const;
    std::string get_payload()            const;
    raw_hash_t  get_hash()               const;
    std::string get_hash_str()           const;
    std::string info(bool verbose=false) const;

    // Static methods:
    static SimpleHashNode create_genesis_node();
    static raw_hash       compute_hash(const SimpleHashNode& node);

    // Static public attributes:
    static const char field_sep = '~';                  /// Field separator for character stream representations of the node.

protected:
    serial_t    serial    = 0;                          /// Node's serial number
    time_t      timestamp = 0;                          /// UNIX timestamp of node creation time
    raw_hash    prev_hash;                              /// Hash of previous node in the chain
    std::string payload   = std::string{"Invalid"};     /// Payload string

    raw_hash  hash_str_to_raw(const std::string& hash_str) const;
};

std::ostream& operator << (std::ostream& strm, const SimpleHashNode& node);
std::istream& operator >> (std::istream& strm, SimpleHashNode& node);



#endif