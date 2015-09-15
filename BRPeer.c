//
//  BRPeer.c
//
//  Created by Aaron Voisine on 9/2/15.
//  Copyright (c) 2015 breadwallet LLC.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "BRPeer.h"

struct BRPeerContext {
    BRPeerStatus status;
    uint32_t version;
    uint64_t nonce;
    const char *useragent;
    uint32_t earliestKeyTime;
    uint32_t lastblock;
    double pingTime;
    int needsFilterUpdate;
    uint32_t currentBlockHeight;
    void (*connected)(BRPeer *peer, void *info);
    void (*disconnected)(BRPeer *peer, BRPeerError, void *info);
    void (*relayedPeers)(BRPeer *peer, BRPeer **peers, size_t count, void *info);
    void (*relayedTx)(BRPeer *peer, BRTransaction *tx, void *info);
    void (*hasTx)(BRPeer *peer, UInt256 txHash, void *info);
    void (*rejectedTx)(BRPeer *peer, UInt256 txHash, uint8_t code, void *info);
    void (*relayedBlock)(BRPeer *peer, BRMerkleBlock *block, void *info);
    const BRTransaction *(*reqeustedTx)(BRPeer *peer, UInt256 txHash, void *info);
    int (*networkIsReachable)(BRPeer *peer, void *info);
    void *info;
};