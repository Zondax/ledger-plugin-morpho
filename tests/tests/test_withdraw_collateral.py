
from web3 import Web3
from tests.utils import run_test, load_contract

contract_morpho_blue = load_contract(
    "bbbbbbbbbb9cc5e90e3b3af64bdaf62c37eeffcb"
)

# Test from https://etherscan.io/tx/0xdbc995c412be15f43422aca334555a1c3f9d1d31c82b89b27a68e75d27f6bed5
def test_withdraw_collateral(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x8720316d0000000000000000000000006b175474e89094c44da98b954eedeac495271d0f0000000000000000000000009d39a5de30e57443bff2a8307a4256c8797a34970000000000000000000000005d916980d5ae1737a8330bf24df812b2911aae25000000000000000000000000870ac11d48b15db9a138cf899d20f13f79ba00bc0000000000000000000000000000000000000000000000000bef55718ad60000000000000000000000000000000000000000000000000e948d77099f5ea74abe0000000000000000000000005c0642fe68fc72cf03f676b28faf5cf97cc0b2650000000000000000000000005c0642fe68fc72cf03f676b28faf5cf97cc0b265"
    run_test(
        contract_morpho_blue, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


