/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "OCTET_STRING.h"
#include "NR_DL-DCCH-Message.h"
#include "NR_RRCReconfiguration.h"
#include "NR_RRCReconfiguration-IEs.h"
#include "NR_CellGroupConfig.h"
#include "NR_DRB-ToAddModList.h"
#include "NR_RRCReconfiguration-v1530-IEs.h"

int main()
{
  // this is an encoded RRC reconfiguration from OAI gNB, inside an
  // DL-DCCH-Message
  uint8_t buf[] = {
      0x06, 0xaa, 0x90, 0x01, 0x02, 0x68, 0x2b, 0x80, 0x08, 0x17, 0x8f, 0xc1,
      0xc5, 0x50, 0x00, 0x1e, 0x50, 0x04, 0xe4, 0x80, 0x14, 0x01, 0x0c, 0x48,
      0xc0, 0x01, 0xc0, 0x14, 0x26, 0x80, 0x00, 0x06, 0x4f, 0xdf, 0x1c, 0x00,
      0x00, 0x00, 0x90, 0xd0, 0xd4, 0x10, 0x13, 0x0b, 0x80, 0x96, 0x00, 0x22,
      0x2f, 0x5d, 0x8e, 0x02, 0x0c, 0x3e, 0x00, 0x16, 0x12, 0x22, 0x2f, 0x5d,
      0x8e, 0x02, 0x0c, 0xbc, 0x00, 0x16, 0x38, 0x06, 0x41, 0x0d, 0xf8, 0xc3,
      0x41, 0xe0, 0xa1, 0x01, 0xf0, 0x08, 0x11, 0xa0, 0xf0, 0x20, 0x00, 0x10,
      0x48, 0x26, 0x06, 0x54, 0x02, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x04,
      0x00, 0x00, 0xdc, 0x51, 0x08, 0x00, 0x02, 0x00, 0x59, 0x40, 0x00, 0x12,
      0x08, 0x00, 0x01, 0x10, 0x24, 0xd0, 0x35, 0x52, 0x4d, 0x40, 0x00, 0x10,
      0x02, 0x02, 0x00, 0x04, 0x00, 0x68, 0x08, 0x00, 0x9d, 0xb2, 0x48, 0xc0,
      0x77, 0x01, 0xd9, 0xe2, 0x6a, 0xf3, 0x40, 0x00, 0x00, 0xa6, 0x84, 0x00,
      0x80, 0x00, 0x96, 0x03, 0x08, 0x30, 0x0b, 0x27, 0x06, 0x08, 0x70, 0x00,
      0xc4, 0xa0, 0x70, 0x00, 0x00, 0x81, 0x7b, 0xd0, 0x04, 0x00, 0x04, 0x00,
      0x00, 0x19, 0x00, 0x05, 0x00, 0x0c, 0xa8, 0x28, 0x03, 0x11, 0x00, 0x54,
      0x00, 0x00, 0x00, 0x00, 0x05, 0x08, 0x02, 0x00, 0x00, 0x51, 0x42, 0x02,
      0x00, 0x0b, 0x18, 0x00, 0x63, 0x7e, 0x02, 0x1d, 0x1c, 0x61, 0x02, 0x02,
      0x7e, 0x00, 0x68, 0x01, 0x00, 0x54, 0x2e, 0x0a, 0x01, 0xc2, 0x11, 0x00,
      0x09, 0x01, 0x00, 0x06, 0x31, 0x21, 0x01, 0x01, 0x00, 0x01, 0x06, 0x06,
      0x01, 0x90, 0x06, 0x00, 0xc8, 0x29, 0x05, 0x01, 0x0c, 0x01, 0x01, 0x82,
      0x22, 0x01, 0x01, 0x81, 0x79, 0x00, 0x06, 0x01, 0x20, 0x41, 0x01, 0x01,
      0x09, 0x7b, 0x00, 0x0d, 0x80, 0x00, 0x0d, 0x04, 0xac, 0x15, 0x03, 0x64,
      0x00, 0x10, 0x02, 0x05, 0xdc, 0x25, 0x17, 0x03, 0x6f, 0x61, 0x69, 0x06,
      0x6d, 0x6e, 0x63, 0x30, 0x39, 0x35, 0x06, 0x6d, 0x63, 0x63, 0x32, 0x30,
      0x38, 0x04, 0x67, 0x70, 0x72, 0x73, 0x12, 0x0a};

  // decode DL-DCCH-Message
  NR_DL_DCCH_Message_t *msg = NULL;
  asn_dec_rval_t dec_rval = uper_decode(NULL, &asn_DEF_NR_DL_DCCH_Message,
                                        (void **)&msg, buf, sizeof(buf), 0, 0);
  assert(dec_rval.code == RC_OK);

  // we can simply print the decoded message, easy!
  xer_fprint(stdout, &asn_DEF_NR_DL_DCCH_Message, msg);

  // it is possible to "look inside". You need to check that you have the right
  // message, though!
  assert(msg->message.present == NR_DL_DCCH_MessageType_PR_c1);
  assert(msg->message.choice.c1->present == NR_DL_DCCH_MessageType__c1_PR_rrcReconfiguration);
  NR_RRCReconfiguration_t *reconfig = msg->message.choice.c1->choice.rrcReconfiguration;

  // we can pick out an element, and print its content (which might be used to
  // proccess data). "IE" is information element
  assert(reconfig->criticalExtensions.present == NR_RRCReconfiguration__criticalExtensions_PR_rrcReconfiguration);
  NR_RRCReconfiguration_IEs_t *ies = reconfig->criticalExtensions.choice.rrcReconfiguration;
  assert(ies->radioBearerConfig != NULL);
  assert(ies->radioBearerConfig->drb_ToAddModList != NULL);
  printf(
      "this reconfiguration triggers the creation of %d data radio bearers\n",
      ies->radioBearerConfig->drb_ToAddModList->list.count);

  // the reconfiguration has again binary data for a CellGroupConfig (which has
  // radio configuration). Decode it again.
  assert(ies->nonCriticalExtension != NULL);
  assert(ies->nonCriticalExtension->masterCellGroup != NULL);
  OCTET_STRING_t *binary_cellGroupConfig = ies->nonCriticalExtension->masterCellGroup;
  NR_CellGroupConfig_t *cellGroupConfig = NULL;
  dec_rval = uper_decode(NULL, &asn_DEF_NR_CellGroupConfig,
                         (void **)&cellGroupConfig, binary_cellGroupConfig->buf,
                         binary_cellGroupConfig->size, 0, 0);
  assert(dec_rval.code == RC_OK);
  xer_fprint(stdout, &asn_DEF_NR_CellGroupConfig, cellGroupConfig);

  // we can also encode a message
  uint8_t cg_buf[2048];
  asn_enc_rval_t enc_rval = uper_encode_to_buffer(
      &asn_DEF_NR_CellGroupConfig, NULL, cellGroupConfig, cg_buf, sizeof(cg_buf));
  assert(enc_rval.encoded > 0);
  // Note!! uper_encode_to_buffer gives us the number of encoded BITS. We have
  // to round to bytes
  size_t cg_buf_size = (enc_rval.encoded + 7) / 8; // round up to full byte
  // compare this is equal to what we decoded from
  assert(cg_buf_size == binary_cellGroupConfig->size);
  for (int i = 0; i < binary_cellGroupConfig->size; ++i) {
    assert(cg_buf[i] == binary_cellGroupConfig->buf[i]);
  }

  // re-encode reconfiguration, with a function that allocates the buffer
  uint8_t *reconf_buf = NULL;
  size_t buf_size = uper_encode_to_new_buffer(&asn_DEF_NR_DL_DCCH_Message, NULL, msg, (void **) &reconf_buf);
  // compare is the same as before. Unlike uper_encode_to_buffer(),
  // uper_encode_to_new_buffer() returns the size in bytes
  assert(buf_size == sizeof(buf));
  for (int i = 0; i < buf_size; ++i) {
    assert(reconf_buf[i] == buf[i]);
  }

  // we have to free memory of allocated objects
  free(reconf_buf);
  ASN_STRUCT_FREE(asn_DEF_NR_DL_DCCH_Message, msg);
  ASN_STRUCT_FREE(asn_DEF_NR_CellGroupConfig, cellGroupConfig);

  return 0;
}
