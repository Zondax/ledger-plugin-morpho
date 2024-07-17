# Technical Specification

> **Warning**
This documentation is a template and shall be updated.

## About

This documentation describes the smart contracts and functions supported by the Morpho plugin.

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
