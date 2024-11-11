
from web3 import Web3
from tests.utils import run_test, load_contract

contract_gunlet_usdt = load_contract(
    "8cb3649114051ca5119141a34c200d65dc0faa73"
)

# Test from https://etherscan.io/tx/0xe62d4450cde19436464765d51771dc602d02154d142379609e37a64c841ef1b1
def test_approve(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x095ea7b3000000000000000000000000e039611e208d11b859e577424c726992c29185b0000000000000000000000000000000000000000000000005068e3200201f1c15"
    run_test(
        contract_gunlet_usdt, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


