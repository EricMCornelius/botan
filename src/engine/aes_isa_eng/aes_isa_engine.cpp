/*
* Engine for AES instructions
* (C) 2009 Jack Lloyd
*
* Distributed under the terms of the Botan license
*/

#include <botan/internal/aes_isa_engine.h>
#include <botan/cpuid.h>

#if defined(BOTAN_HAS_AES_INTEL)
  #include <botan/aes_intel.h>
#endif

namespace Botan {

BlockCipher*
AES_ISA_Engine::find_block_cipher(const SCAN_Name& request,
                                  Algorithm_Factory&) const
   {
#if defined(BOTAN_HAS_AES_INTEL)
   if(CPUID::has_aes_intel())
      {
      if(request.algo_name() == "AES-128")
         return new AES_128_Intel;
      if(request.algo_name() == "AES-192")
         return new AES_192_Intel;
      if(request.algo_name() == "AES-256")
         return new AES_256_Intel;
      }
#endif

   return 0;
   }

}