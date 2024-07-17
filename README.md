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
| Ethereum | EthereumBundlerV2               | 0x4095F064B8d3c3548A3bebfd0Bbfd04750E30077 |
| Ethereum | EthereumBundler                 | 0xa7995f71aa11525db02fc2473c37dee5dbf55107 |
| Ethereum | AaveV2MigrationBundler          | 0xb3dcc75db379925edfd3007511a8ce0cb4aa8e76 |
| Ethereum | AaveV3MigrationBundler          | 0x98ccb155e86bb478d514a827d16f58c6912f9bdc |
| Ethereum | AaveV3OptimizerMigrationBundler | 0x16f38d2e764e7bebf625a8e995b34968226d2f9c |
| Ethereum | CompoundV2MigrationBundler      | 0x26bf52a84360ad3d01d7cdc28fc2ddc04d8c8647 |
| Ethereum | CompoundV3MigrationBundler      | 0x3a0e2e9fb9c95fbc843daf166276c90b6c479558 |
| Ethereum | CompoundV3MigrationBundlerV2    | 0x1f8076e2eb6f10b12e6886f30d4909a91969f7da |
| Ethereum | ChainAgnosticBundlerV2          | 0x23055618898e202386e6c13955a58D3C68200BFB |
| Ethereum | CompoundV2MigrationBundlerV2    | 0x123f3167a416cA19365dE03a65e0AF3532af7223 |
| Ethereum | AaveV3MigrationBundlerV2        | 0xcAe2929baBc60Be34818EaA5F40bF69265677108 |


## Functions

For the smart contracts implemented, the functions covered by the plugin shall be described here:

|    Function   | Selector  | Displayed Parameters |
|    ---        | ---       | --- |
| Multicall*     | `0xac9650d8`| <table><tbody> <tr><td><code>bytes[] call</code></td></tr> </tbody></table> |

*For byte arrays bigger then 32, plugin is showing the first and last 16 bytes in "16...16" format, due to memory limitations. Plugin will process a maximum of 3 calls in the method.

## How to build

Ledger's recommended [plugin guide](https://developers.ledger.com/docs/dapp/embedded-plugin/code-overview/) is out-dated and doesn't work since they introduced a lot of new changes. Here's a simple way to get started with this repo:
1. Clone this repo (along with git submodules)
2. Install [Xquartz](https://www.xquartz.org/) and make sure you have enabled "Allow connections from network clients" enabled under "Security" settings.
3. Install and start Docker (Note: If Docker is already running, quit and start it after starting Xquartz, otherwise docker cannot connect to Xquartz).
4. Install the [Ledger Dev Tools VS Code plugin](https://marketplace.visualstudio.com/items?itemName=LedgerHQ.ledger-dev-tools#:~:text=ledger%2Dvscode%2Dextension,Plus%2C%20Nano%20X%2C%20Stax) and makes sure it's enabled
5. Once you have installed the plugin and open the repo, the plugin should by default try to create and start the containers. If it doesn't, you can simply click "Update Container" under "Ledger Dev Tools" in the Activity Side Bar on VS Code.
6. On the "Ledger Dev Tools" side bar, Select a target and then click on Build. 
7. Once build is complete, click on "Run tests" to run the tests
