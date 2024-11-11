
from web3 import Web3
from tests.utils import run_test, load_contract

contract_re7_weth = load_contract(
    "78fc2c2ed1a4cdb5402365934ae5648adad094d0"
)

# Test from https://etherscan.io/tx/0x01062eec602faa1abff730e565e4b9a4ae29cd68552c876206b3fe8a983fb060
def test_withdraw(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xb460af940000000000000000000000000000000000000000000000000000000000000001000000000000000000000000cc771952fde840e30c6802734e5ad20479c2959f000000000000000000000000cc771952fde840e30c6802734e5ad20479c2959f"
    run_test(
        contract_re7_weth, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


