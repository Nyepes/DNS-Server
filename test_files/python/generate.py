import dns.message
import dns.query
import sys

# Replace 'example.com' with your desired domain name
domain_name = 'www.google.com'

# Create a DNS query message
query = dns.message.make_query(domain_name, dns.rdatatype.A)
response = dns.query.udp(query, '192.48.79.30', timeout=5)
# Convert the query to raw bytes
query.id = 40254
raw_bytes = query.to_wire()
print(f'query: {raw_bytes}', file=sys.stderr)
print(f'query: {raw_bytes.hex()}', file=sys.stderr)
print(f'id: {response.id}', file=sys.stderr)
print(f'flags: {response.flags}', file=sys.stderr)
print(f'question: {response.question}', file=sys.stderr)
print(f'answer: {response.answer}', file=sys.stderr)
print(f'authorities: {response.authority}', file=sys.stderr)
print(f'additional: {response.additional}', file=sys.stderr)

for ans in response.answer:
    print(ans.ttl)
    # print(f'additional: {ans.ttl}\n', file=sys.stderr)


# print(f'answer: {response.answer}', file=sys.stderr)

# print(raw_bytes.hex())
# print("Raw bytes of DNS query for", domain_name)
print(f'length: {len(raw_bytes.hex())}', file=sys.stderr)
ascii_string = ""
hex_string = raw_bytes.hex()
byte_data = bytes.fromhex(hex_string)

# with open("../test4.txt", 'wb') as file:
#     file.write(byte_data)