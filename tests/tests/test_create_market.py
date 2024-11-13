
from web3 import Web3
from tests.utils import run_test, load_contract

contract_morpho_blue = load_contract(
    "bbbbbbbbbb9cc5e90e3b3af64bdaf62c37eeffcb"
)

# Test from https://etherscan.io/tx/0x632bca32e2d631652e798485c4467ff04e0461e80bb8db4c8fa2d475231f5385
def test_create_market(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x8c1358a20000000000000000000000006b175474e89094c44da98b954eedeac495271d0f0000000000000000000000004a8036efa1307f1ca82d932c0895faa18db0c9ee00000000000000000000000038d130cee60cda080a3b3ac94c79c34b6fc919a7000000000000000000000000870ac11d48b15db9a138cf899d20f13f79ba00bc0000000000000000000000000000000000000000000000000cb2bba6f17b8000"
    run_test(
        contract_morpho_blue, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


