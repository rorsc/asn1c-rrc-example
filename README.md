# ASN.1 3GPP encoding&decoding demo

This repository demonstrates how
[OpenAirInterface](https://gitlab.eurecom.fr/oai/openairinterface5g) manages
ASN.1 grammar compilation, and provides an example of encoding, decoding, and
printing a sample message, namely an RRC reconfiguration of 3GPP 38.331 Rel
17.3.

It orients on OAI tag `2024.w18` (`82bd07ebd54b77fab793d5db1d3926f7144e91d2`).

## Prerequisites

This repository uses cmake to build the code. It assumes that you have asn1c
already installed. If not, install it from
[here](https://github.com/mouse07410/asn1c).

## Build

In order to build the project, run the following commands:

```bash
mkdir build
cmake . -GNinja -DCMAKE_BUILD_TYPE=Debug -DADDRESS_SANITIZER=ON -Bbuild
ninja -C build
```

## asn1c compilation explanation

OAI creates the ASN.1 source files during compilation time. The root-level
`CMakeLists.txt` checks that `asn1c` is present, and that it supports the
options used to generate ASN.1 sources.

All ASN.1 target-related cmake code is in `asn1c/CMakeLists.txt`. First, it
sets the required ASN.1 grammar version to support the selection of different
grammars before compilation. Depending on the version, a specific `.cmake` file
is loaded (so in this repository, since we select RRC version 17.3, we include
`asn1c/nr-rrc-17.3.0.cmake`). This file references a `.asn1` file for the ASN.1
grammar to be compiled (`NR_RRC_GRAMMAR`), and the source (`nr_rrc_source`) and
header files (`nr_rrc_headers`) that will be generated when running `asn1c` on
the chosen grammar. This allows cmake to both generate the sources during
compilation time, verify that they are complete, and compile into a target.

With these variables defined, cmake is able to instruct the build tool (e.g.,
`ninja`, `make`) to generate and compile sources. First, `add_custom_command()`
is used to generate the ASN.1 sources through an invocation of `asn1c`,
specifiying the grammar as a dependency and the source and header files as
output of this step. This is then used to define two libraries,
`asn1_nr_rrc_hdrs` and `asn1_nr_rrc`, to group the headers and the source files
to be compiled.

It is possible to only generate the source files by building only the header
target file

```bash
mkdir build && cmake . -GNinja -Bbuild && ninja -Cbuild asn1_nr_rrc_hdrs
```

or to go one step further and compile all source files

```bash
mkdir build && cmake . -GNinja -Bbuild && ninja -Cbuild asn1_nr_rrc
```

In cmake, an executable can use the generated ASN.1 sources and headers by
linking to `asn1_nr_rrc`:

```
target_link_libraries(<execurable-or-library> PRIVATE asn1_nr_rrc)
```

## RRC reconfiguration explanation

An example is provided to show how to decode, encode, and print ASN.1 in C in
file `rrc-reconfig.c`. This file is pretty much analogous to the [`asn1c` quick
start guide](http://lionet.info/asn1c/documentation.html), with the difference
that the used grammar is the full 3GPP 5G NR RRC spec instead of a simple sample
grammar.

Specifically, the example uses a pre-defined encoded RRC message to illustrate
the usage of `asn1c`. The encoded RRC message was taken from the RAN by running
the gNB and connecting a UE. Specifically, I added a patch (valid as of
commit `82bd07ebd54b77fab793d5db1d3926f7144e91d2`, might be different for other
versions) that prints the encoded RRC reconfiguration message:

```patch
diff --git a/openair2/RRC/NR/rrc_gNB.c b/openair2/RRC/NR/rrc_gNB.c
index 1e6915624b..062be78c91 100644
--- a/openair2/RRC/NR/rrc_gNB.c
+++ b/openair2/RRC/NR/rrc_gNB.c
@@ -563,6 +563,11 @@ void rrc_gNB_generate_dedicatedRRCReconfiguration(const protocol_ctxt_t *const c
         ctxt_pP->module_id,
         DCCH);

+  printf("reconfiguration bytes:\n");
+  for (int i = 0 ; i < size; ++i)
+    printf("0x%02x, ", buffer[i]);
+  printf("\n");
+
   nr_rrc_transfer_protected_rrc_message(rrc, ue_p, DCCH, buffer, size);
 }
  
```

The resulting bytes were copy-pasted into `rrc-reconfig.c`. From here, I then
used the asn1c API to do all operations; the example has explanatory comments.
You should pay attention to how to

- decode a message with `uper_decode()`, providing the type of message,
- print a message in XML format using `xer_fprint()`, again providing the
  type of the message to be printed,
- access specific fields and read values,
- encode a message using the two `asn1c` encoding functions
  `uper_encode_to_buffer()` (encode to existing buffer) and
  `uper_encode_to_new_buffer()` (encode to buffer that `asn1c` allocates on the
  heap for the user), and
- how to properly free allocated memory.

For instance, in an O-RAN architecture with near-RT RIC and gNB, an xApp might
request to be informed about RRC Reconfiguration messages sent to UEs. In that
case, the gNB would send indication messages with the encoded stream of bytes;
the xApp will have the bytes as in `rrc-reconfig.c`, variable `buf`, and can
then decode and inspect the message.

Regarding the allocation of memory, the project comes with an option for the
address sanitizer so that memory leaks can be identified.
