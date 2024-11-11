
from web3 import Web3
from tests.utils import run_test, load_contract

contract_flagship_eth = load_contract(
    "38989bba00bdf8181f4082995b3deae96163ac5d"
)

# Test from https://etherscan.io/tx/0x01062eec602faa1abff730e565e4b9a4ae29cd68552c876206b3fe8a983fb060
def test_deposit(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x6e553f6500000000000000000000000000000000000000000000000000b1a2bc2ec500000000000000000000000000008949ac8bae1389179cc222d9ec21a1fc2f69c786"
    run_test(
        contract_flagship_eth, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


