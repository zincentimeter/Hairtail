#include <graphenelib/asset.h>
#include <graphenelib/contract.hpp>
#include <graphenelib/contract_asset.hpp>
#include <graphenelib/crypto.h>
#include <graphenelib/dispatcher.hpp>
#include <graphenelib/global.h>
#include <graphenelib/multi_index.hpp>
#include <graphenelib/print.hpp>
#include <graphenelib/system.h>
#include <graphenelib/types.h>

#include<cmath>
#include<map>

using namespace graphene;

class module : public contract
{
  public:
    module(uint64_t id)
        : contract(id)
        , data(_self, _self)
    {
    }
	
  	std::map<std::string, uint64_t> address;
  	
  	
    // not at abi payable
    // @abi action
    void push(std::string domain, uint32_t page_v, uint32_t unique_v) {
    	uint32_t time_stamp = get_head_block_time();
    	uint64_t owner = get_trx_sender();
    	auto it = data.find(owner);
    	if (it == data.end()) {
    		data.emplace(owner, [&](auto &o) {
    			graphene_assert(o.pv.empty(), "Rua");
    			graphene_assert(o.uv.empty(), "2Rua2");
	            o.publisher = owner;
	            o.domain = domain;
	            o.pv.emplace_back(page_v);
	            o.uv.emplace_back(unique_v);
	            o.timestamp.emplace_back(time_stamp);
	            address[domain] = owner;
	        });
		}
    	else {
    		data.modify(it, 0, [&](auto &o) {
    			graphene_assert(o.publisher == owner, "foooo");
    			graphene_assert(o.domain == domain, "3foooooo3");
    			o.pv.emplace_back(page_v);
    			o.uv.emplace_back(unique_v);
	            o.timestamp.emplace_back(time_stamp);
			});
		}
		
		
		printf("%d %d %d %d %s",page_v,unique_v,time_stamp,address[domain],domain);
		
		
	}
    
    // @abi action
    void pull(std::string domain, const uint32_t BEGIN, const uint32_t END) {//counted in second
    	vector<uint32_t> x, y1, y2; //timestamp, pv, uv
    	uint64_t refer = address[domain];
    	auto it = data.find(refer);
    	graphene_assert(it != data.end(), "No Result! ");
    	uint32_t cnt = 0, now;
    	for (;it->timestamp[cnt] < BEGIN; ++cnt);
    	for (now=cnt; BEGIN<=it->timestamp[now] && it->timestamp[now]<=END; ++now) {
    		x[now-cnt] = it->timestamp[now];
    		y1[now-cnt] = it->pv[now];
    		y2[now-cnt] = it->uv[now];
		}
		
		
		for (uint32_t i=0; i<=now-cnt; ++i) printf("[%u, %u, %u] ",x[i], y1[i], y2[i]);
		
		
	}
    
  private:
    //@abi table datum i64
    struct datum {
        uint64_t                publisher;
        std::string             domain;
        std::vector<uint32_t>   pv;
        std::vector<uint32_t>   uv;
        std::vector<uint32_t>   timestamp;

        uint64_t primary_key() const { return publisher; }

        GRAPHENE_SERIALIZE(datum, (publisher)(domain)(pv)(uv)(timestamp))
    };
    typedef graphene::multi_index<N(datum), datum> datum_index;    //**********

    struct account {
        uint64_t    account_id; 
        int64_t     amount;

        GRAPHENE_SERIALIZE(account, (account_id)(amount))
    };
    
    datum_index     data;
};

GRAPHENE_ABI(module, (push)(pull))

