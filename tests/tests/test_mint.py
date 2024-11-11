
from web3 import Web3
from tests.utils import run_test, load_contract

contract_re7_weth = load_contract(
    "78fc2c2ed1a4cdb5402365934ae5648adad094d0"
)

def test_mint(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x94bf804d00000000000000000000000000000000000000000000000000000000000000230000000000000000000000001d1479c185d32eb90533a08b36b3cfa5f84a0e6b"
    run_test(
        contract_re7_weth, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )


