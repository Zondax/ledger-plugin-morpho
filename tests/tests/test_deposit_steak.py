
from web3 import Web3
from tests.utils import run_test, load_contract

contract_steakhouse = load_contract(
    "beef047a543e45807105e51a8bbefcc5950fcfba"
)

# Test from https://etherscan.io/tx/0x46f43ae99846c1155f4d9790af13ae82aa4af930364bc7ca692ff999ad006dfa
def test_deposit_steak(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x6e553f650000000000000000000000000000000000000000000000000000000000989680000000000000000000000000b2642d4f596311ea15acd544ba048815421fb896"
    run_test(
        contract_steakhouse, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


