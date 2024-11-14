
from web3 import Web3
from tests.utils import run_test, load_contract

contract_morpho_blue = load_contract(
    "bbbbbbbbbb9cc5e90e3b3af64bdaf62c37eeffcb"
)

# Test from https://etherscan.io/tx/0x754f0081cb8ac906c2326829442c21659d64461aac23d58e1705a2e15242a5b4
def test_set_authorization(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xeecea00000000000000000000000000027358775f25f5061294c0f888f65977cb02273ad0000000000000000000000000000000000000000000000000000000000000001"
    run_test(
        contract_morpho_blue, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


