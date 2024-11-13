
from web3 import Web3
from tests.utils import run_test, load_contract

contract_morpho_blue = load_contract(
    "bbbbbbbbbb9cc5e90e3b3af64bdaf62c37eeffcb"
)

# Test from https://basescan.org/tx/0xbb94bff0a24b7a9cc40316a14d7267ce3be27c0af5d70be6e6c7fa1a17d744ba
def test_flash_loan(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xe0232b42000000000000000000000000833589fcd6edb6e08f4c7c32d4f71b54bda02913000000000000000000000000000000000000000000000000000012309ce5400000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000020000000000000000000000000833589fcd6edb6e08f4c7c32d4f71b54bda02913"
    run_test(
        contract_morpho_blue, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


