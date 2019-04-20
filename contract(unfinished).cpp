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

using namespace graphene;

class module : public contract
{
  public:
    module(uint64_t id)
        : contract(id)
        , data(_self, _self)
    {
    }

    // @abi action
    // not at abi payable
    void push(std::string domain, uint32_t pv, uint32_t uv) {
    	uint32_t timestamp = get_head_block_time();
    	uint64_t owner = get_trx_sender();
    	auto it = data.find(owner);
    	if (it == data.end()) {
    		data.emplace(owner, [&](auto &o) {
    			graphene_assert(o.page_v.empty(), "Ruaaaaa");
    			graphene_assert(o.unique_v.empty(), "Ruaaa");
	            o.publisher = owner;
	            o.domain = domain;
	            o.page_v.emplace_back(pv);
	            o.unique_v.emplace_back(uv);
	            o.timestamp = timestamp;
	        });
		}
    	else {
    		data.modify(it, 0, [&](auto &o) {
    			graphene_assert(o.publisher == owner, "foooo");
    			graphene_assert(o.domain == domain, "foooooo");
    			o.page_v.emplace_back(pv);
    			o.unique_v.emplace_back(uv);
	            o.timestamp = timestamp;
			});
		}
		printf("%d %d %d\n",pv,uv,timestamp);
	}
    
  private:
    //@abi table datum i64
    struct datum {
        uint64_t                publisher;
        std::string             domain;
        contract_asset          total_amount;
        std::vector<uint32_t>   page_v;
        std::vector<uint32_t>   unique_v;
        uint32_t                timestamp;

        uint64_t primary_key() const { return publisher; }

        GRAPHENE_SERIALIZE(datum, (publisher)(domain)(total_amount)(page_v)(unique_v))
    };
    typedef graphene::multi_index<N(datum), datum> datum_index;    //**********

    struct account {
        uint64_t    account_id; 
        int64_t     amount;

        GRAPHENE_SERIALIZE(account, (account_id)(amount))
    };
    
    datum_index     data;
};

GRAPHENE_ABI(module, (push))

