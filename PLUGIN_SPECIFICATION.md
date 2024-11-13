# Technical Specification

## About

This documentation describes the smart contracts and functions supported by the Morpho plugin.

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

