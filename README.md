# app-plugin-morpho

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
---

![zondax_light](docs/zondax_light.png#gh-light-mode-only)
![zondax_dark](docs/zondax_dark.png#gh-dark-mode-only)

_Please visit our website at [zondax.ch](https://www.zondax.ch)_

---

This is a plugin for the Ethereum application which helps parsing and displaying relevant information when signing a Morpho smart contract.

- Ledger Nano S/S+/X/Stax Morpho plugin
- Specs / Documentation
- Ragger tests

## ATTENTION

Please:

- **Do not use in production**
- **Do not use a Ledger device with funds for development purposes.**
- **Have a separate and marked device that is used ONLY for development and testing**

## Documentation

Need more information about the interface, the architecture, or general stuff about ethereum plugins? You can find more about them in the ethereum-app documentation.

## Smart Contracts

Smart contracts covered by the plugin shall be described here:

|  Network | Smart Contract                  | Address                                    |
|   ----   |      ----                       |   ---                                      |
| Ethereum | Public Allocator                | 0xfd32fA2ca22c76dD6E550706Ad913FC6CE91c75D |
| Base     | Public Allocator                | 0xA090dD1a701408Df1d4d0B85b716c87565f90467 |
| Ethereum | MorphoBlue                      | 0xBBBBBbbBBb9cC5e90e3b3Af64bdAF62C37EEFFCb |
| Base     | MorphoBlue                      | 0xBBBBBbbBBb9cC5e90e3b3Af64bdAF62C37EEFFCb |
| Ethereum | MetaMorpho Factory*             | 0xA9c3D3a366466Fa809d1Ae982Fb2c46E5fC41101 |
| Base     | MetaMorpho Factory*             | 0xA9c3D3a366466Fa809d1Ae982Fb2c46E5fC41101 |

*Plugin will support methods for the vaults created by the factory.

## Functions

For the smart contracts implemented, the functions covered by the plugin shall be described here:

|    Function               | Selector      | Displayed Parameters |
|    ---                    | ---           | --- |
| reallocateTo              | 0x833947fd    |  <table><tbody> <tr><td><code>address vault</code></td></tr> </tbody></table> |
| repay                     | 0x20b76e81    | <table><tbody> <tr><td><code>uint256 assets</code></td></tr><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address onBehalf</code></td></tr> </tbody></table> |
| supplyCollateral          | 0x238d6579    | <table><tbody> <tr><td><code>uint256 assets</code></td></tr><tr><td><code>address onBehalf</code></td></tr> </tbody></table> |
| borrow                    | 0x50d8cd4b    | <table><tbody> <tr><td><code>uint256 assets</code></td></tr><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address onBehalf</code></td></tr> </tbody></table> |
| withdraw                  | 0x5c2bea49    | <table><tbody> <tr><td><code>uint256 assets</code></td></tr><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address onBehalf</code></td></tr> </tbody></table> |
| setAuthorizationWithSig   | 0x8069218f    | <table><tbody> <tr><td><code>address authorizer</code></td></tr><tr><td><code>address authorized</code></td></tr><tr><td><code>bool isAuthorized</code></td></tr> </tbody></table> |
| withdrawCollateral        | 0x8720316d    | <table><tbody> <tr><td><code>uint256 assets</code></td></tr><tr><td><code>address onBehalf</code></td></tr> </tbody></table> |
| createMarket              | 0x8c1358a2    | <table><tbody> <tr><td><code>address loan_token</code></td></tr><tr><td><code>address collateral_token</code></td></tr> </tbody></table> |
| supply                    | 0xa99aad89    | <table><tbody> <tr><td><code>uint256 assets</code></td></tr><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address onBehalf</code></td></tr> </tbody></table> |
| flashLoan                 | 0xe0232b42    | <table><tbody> <tr><td><code>address token</code></td></tr><tr><td><code>uint256 assets</code></td></tr> </tbody></table> |
| setAuthorization          | 0xeecea000    | <table><tbody><tr><td><code>address authorized</code></td></tr><tr><td><code>bool isAuthorized</code></td></tr> </tbody></table> |
| deposit                   | 0x6e553f65   | <table><tbody><tr><td><code>uint256 assets</code></td></tr><tr><td><code>address receiver</code></td></tr> </tbody></table> |
| mint                      | 0x94bf804d    | <table><tbody><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address receiver</code></td></tr> </tbody></table> |
| redeem                    | 0xba087652    | <table><tbody><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address receiver</code></td></tr> <tr><td><code>address owner</code></td></tr></tbody></table> |
| withdraw                  | 0xb460af94    | <table><tbody><tr><td><code>uint256 assets</code></td></tr><tr><td><code>address receiver</code></td></tr> <tr><td><code>address owner</code></td></tr></tbody></table> |
| approve                   | 0x095ea7b3     | <table><tbody><tr><td><code>uint256 shares</code></td></tr><tr><td><code>address spender</code></td></tr> </tbody></table> |


## How to build

Ledger's recommended [plugin guide](https://developers.ledger.com/docs/dapp/embedded-plugin/code-overview/) is out-dated and doesn't work since they introduced a lot of new changes. Here's a simple way to get started with this repo:
1. Clone this repo (along with git submodules)
2. Install [Xquartz](https://www.xquartz.org/) and make sure you have enabled "Allow connections from network clients" enabled under "Security" settings.
3. Install and start Docker (Note: If Docker is already running, quit and start it after starting Xquartz, otherwise docker cannot connect to Xquartz).
4. Install the [Ledger Dev Tools VS Code plugin](https://marketplace.visualstudio.com/items?itemName=LedgerHQ.ledger-dev-tools#:~:text=ledger%2Dvscode%2Dextension,Plus%2C%20Nano%20X%2C%20Stax) and makes sure it's enabled
5. Once you have installed the plugin and open the repo, the plugin should by default try to create and start the containers. If it doesn't, you can simply click "Update Container" under "Ledger Dev Tools" in the Activity Side Bar on VS Code.
6. On the "Ledger Dev Tools" side bar, Select a target and then click on Build. 
7. Once build is complete, click on "Run tests" to run the tests
