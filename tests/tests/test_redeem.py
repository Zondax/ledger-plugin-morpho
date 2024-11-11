
from web3 import Web3
from tests.utils import run_test, load_contract

contract_erc20 = load_contract(
    "2c25f6c25770ffec5959d34b94bf898865e5d6b1"
)

# Test from https://etherscan.io/tx/0x8e4c71d28a923422941a6a2f3d6bcd48d99833a4bda30cca79e70f3ac74d560e
def test_redeem(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xba0876520000000000000000000000000000000000000000000000000d5d23b22d6e377400000000000000000000000003b94c89a9226e959f7b6e282d7d35520482cd3100000000000000000000000003b94c89a9226e959f7b6e282d7d35520482cd31"
    run_test(
        contract_erc20, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


