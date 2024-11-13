
from web3 import Web3
from tests.utils import run_test, load_contract

contract_morpho_blue = load_contract(
    "bbbbbbbbbb9cc5e90e3b3af64bdaf62c37eeffcb"
)

# Test from https://etherscan.io/tx/0x21f5cc0d111883dc336004c3ec98e0b19dbfcce28644865944623d4b30e2f744
def test_repay(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x20b76e81000000000000000000000000a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48000000000000000000000000e00bd3df25fb187d6abbb620b3dfd19839947b810000000000000000000000009c0174fe7748f318dcb7300b93b170b6026280b0000000000000000000000000870ac11d48b15db9a138cf899d20f13f79ba00bc0000000000000000000000000000000000000000000000000cb2bba6f17b80000000000000000000000000000000000000000000000000000000000000000bb8000000000000000000000000000000000000000000000000000000000000000000000000000000000000000017e7bb9fe7983947fdcf02c1e3d8e6c92c21da5400000000000000000000000000000000000000000000000000000000000001200000000000000000000000000000000000000000000000000000000000000000"
    run_test(
        contract_morpho_blue, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )

