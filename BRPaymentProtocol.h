//
//  BRPaymentProtocol.h
//
//  Created by Aaron Voisine on 9/7/15.
//  Copyright (c) 2015 breadwallet LLC
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

#ifndef BRPaymentProtocol_h
#define BRPaymentProtocol_h

#include "BRTransaction.h"
#include <stdint.h>

// BIP70 payment protocol: https://github.com/bitcoin/bips/blob/master/bip-0070.mediawiki

typedef struct {
    char *network; // "main" or "test", default is "main"
    BRTxOutput *outputs; // where to send payments, outputs->amount defaults to 0
    size_t outputsCount;
    uint64_t time; // request creation time, seconds since unix epoch, optional
    uint64_t expires; // when this request should be considered invalid, optional
    char *memo; // human-readable description of request for the customer, optional
    char *paymentURL; // url to send payment and get payment ack, optional
    uint8_t *merchantData; // arbitrary data to include in the payment message, optional
    size_t merchDataLen;
} BRPaymentProtocolDetails;

// buf must contain a serialized details struct, result must be freed by calling BRPayementProtocolDetailsFree()
BRPaymentProtocolDetails *BRPaymentProtocolDetailsDeserialize(const uint8_t *buf, size_t len);

// writes serialized details struct to buf, returns number of bytes written, or total len needed if buf is NULL
size_t BRPaymentProtocolDetailsSerialize(BRPaymentProtocolDetails *details, uint8_t *buf, size_t len);

// frees memory allocated for details struct
void BRPayementProtocolDetailsFree(BRPaymentProtocolDetails *details);

typedef struct {
    uint32_t version; // default is 1
    char *pkiType; // none / x509+sha256 / x509+sha1, default is "none"
    uint8_t *pkiData; // depends on pkiType, optional
    size_t pkiLen;
    BRPaymentProtocolDetails *details; // required
    uint8_t *signature; // pki-dependent signature, optional
    size_t sigLen;
} BRPaymentProtocolRequest;

// buf must contain a serialized request struct, result must be freed by calling BRPaymentProtocolRequestFree()
BRPaymentProtocolRequest *BRPaymentProtocolRequestDeserialize(const uint8_t *buf, size_t len);

// writes serialized request struct to buf, returns number of bytes written, or total len needed if buf is NULL
size_t BRPaymentProtocolRrequestSerialize(BRPaymentProtocolRequest *request, uint8_t *buf, size_t len);

// writes the DER encoded certificate corresponding to index to cert, returns the number of bytes written to cert, or
// the total certLen needed if cert is NULL, returns 0 if index of out-of-bounds
size_t BRPaymentProtocolRequestCert(BRPaymentProtocolRequest *request, uint8_t *cert, size_t certLen, size_t index);

// writes the hash of the request to md needed to sign or verify the request, returns the number of bytes written, or
// the total bytes needed if md is NULL
size_t BRPaymentProtocolRequestDigest(BRPaymentProtocolRequest *request, uint8_t *md, size_t mdLen);

// frees memory allocated for request struct
void BRPayementProtocolRequestFree(BRPaymentProtocolRequest *request);

typedef struct {
    uint8_t *merchantData; // from request->details->merchantData, optional
    size_t merchDataLen;
    BRTransaction *transactions; // array of signed BRTransaction structs to satisfy outputs from details
    size_t txCount;
    BRTxOutput *refunds; // where to send refunds, if a refund is necessary, refunds->amount defaults to 0
    size_t refundsCount;
    char *memo; // human-readable message for the merchant, optional
} BRPaymentProtocolPayment;

// buf must contain a serialized payment struct, result must be freed by calling BRPayementProtocolPaymentFree()
BRPaymentProtocolPayment *BRPaymentProtocolPaymentDeserialize(const uint8_t *buf, size_t len);

// writes serialized payment struct to buf, returns number of bytes written, or total len needed if buf is NULL
size_t BRPaymentProtocolPaymentSerialize(BRPaymentProtocolPayment *payment, uint8_t *buf, size_t len);

// frees memory allocated for payment struct
void BRPayementProtocolPaymentFree(BRPaymentProtocolPayment *payment);

typedef struct {
    BRPaymentProtocolPayment *payment; // payment message that triggered this ack, required
    char *memo; // human-readable message for customer, optional
} BRPaymentProtocolACK;

// buf must contain a serialized ACK struct, result must be freed by calling BRPayementProtocolACKFree()
BRPaymentProtocolPayment *BRPaymentProtocolACKDeserialize(const uint8_t *buf, size_t len);

// writes serialized ACK struct to buf, returns number of bytes written, or total len needed if buf is NULL
size_t BRPaymentProtocolACKSerialize(BRPaymentProtocolPayment *payment, uint8_t *buf, size_t len);

// frees memory allocated for ACK struct
void BRPayementProtocolACKFree(BRPaymentProtocolPayment *payment);

#endif // BRPaymentProtocol_h