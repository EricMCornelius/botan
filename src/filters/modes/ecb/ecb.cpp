/*
* ECB Mode
* (C) 1999-2009 Jack Lloyd
*
* Distributed under the terms of the Botan license
*/

#include <botan/ecb.h>

namespace Botan {

/*
* ECB_Encryption Constructor
*/
ECB_Encryption::ECB_Encryption(BlockCipher* ciph,
                               BlockCipherModePaddingMethod* pad) :
   Buffered_Filter(ciph->BLOCK_SIZE * ciph->parallelism(), 0)
   {
   cipher = ciph;
   padder = pad;

   temp.resize(buffered_block_size());
   }

/*
* ECB_Encryption Constructor
*/
ECB_Encryption::ECB_Encryption(BlockCipher* ciph,
                               BlockCipherModePaddingMethod* pad,
                               const SymmetricKey& key) :
   Buffered_Filter(ciph->BLOCK_SIZE * ciph->parallelism(), 0)
   {
   cipher = ciph;
   padder = pad;

   temp.resize(buffered_block_size());

   cipher->set_key(key);
   }

/*
* ECB_Encryption Destructor
*/
ECB_Encryption::~ECB_Encryption()
   {
   delete cipher;
   delete padder;
   }

/*
* Return an ECB mode name
*/
std::string ECB_Encryption::name() const
   {
   return (cipher->name() + "/ECB/" + padder->name());
   }

/*
* Encrypt in ECB mode
*/
void ECB_Encryption::write(const byte input[], u32bit length)
   {
   Buffered_Filter::write(input, length);
   }

/*
* Finish encrypting in ECB mode
*/
void ECB_Encryption::end_msg()
   {
   u32bit last_block = current_position() % cipher->BLOCK_SIZE;

   SecureVector<byte> padding(cipher->BLOCK_SIZE);
   padder->pad(padding, padding.size(), last_block);

   u32bit pad_bytes = padder->pad_bytes(cipher->BLOCK_SIZE, last_block);

   if(pad_bytes)
      Buffered_Filter::write(padding, pad_bytes);
   Buffered_Filter::end_msg();
   }

void ECB_Encryption::buffered_block(const byte input[], u32bit input_length)
   {
   const u32bit blocks_in_temp = temp.size() / cipher->BLOCK_SIZE;
   u32bit blocks = input_length / cipher->BLOCK_SIZE;

   while(blocks)
      {
      u32bit to_proc = std::min<u32bit>(blocks, blocks_in_temp);

      cipher->encrypt_n(input, &temp[0], to_proc);

      send(temp, to_proc * cipher->BLOCK_SIZE);

      input += to_proc * cipher->BLOCK_SIZE;
      blocks -= to_proc;
      }
   }

void ECB_Encryption::buffered_final(const byte input[], u32bit input_length)
   {
   if(input_length % cipher->BLOCK_SIZE == 0)
      buffered_block(input, input_length);
   else if(input_length != 0)
      throw Encoding_Error(name() + ": Did not pad to full blocksize");
   }

/*
* ECB_Decryption Constructor
*/
ECB_Decryption::ECB_Decryption(BlockCipher* ciph,
                               BlockCipherModePaddingMethod* pad) :
   Buffered_Filter(ciph->BLOCK_SIZE * ciph->parallelism(), 1)
   {
   cipher = ciph;
   padder = pad;

   temp.resize(buffered_block_size());
   }

/*
* ECB_Decryption Constructor
*/
ECB_Decryption::ECB_Decryption(BlockCipher* ciph,
                               BlockCipherModePaddingMethod* pad,
                               const SymmetricKey& key) :
   Buffered_Filter(ciph->BLOCK_SIZE * ciph->parallelism(), 1)
   {
   cipher = ciph;
   padder = pad;

   temp.resize(buffered_block_size());

   cipher->set_key(key);
   }

/*
* ECB_Decryption Destructor
*/
ECB_Decryption::~ECB_Decryption()
   {
   delete cipher;
   delete padder;
   }

/*
* Return an ECB mode name
*/
std::string ECB_Decryption::name() const
   {
   return (cipher->name() + "/ECB/" + padder->name());
   }

/*
* Decrypt in ECB mode
*/
void ECB_Decryption::write(const byte input[], u32bit length)
   {
   Buffered_Filter::write(input, length);
   }

/*
* Finish decrypting in ECB mode
*/
void ECB_Decryption::end_msg()
   {
   Buffered_Filter::end_msg();
   }

/*
* Decrypt in ECB mode
*/
void ECB_Decryption::buffered_block(const byte input[], u32bit length)
   {
   const u32bit blocks_in_temp = temp.size() / cipher->BLOCK_SIZE;
   u32bit blocks = length / cipher->BLOCK_SIZE;

   while(blocks)
      {
      u32bit to_proc = std::min<u32bit>(blocks, blocks_in_temp);

      cipher->decrypt_n(input, &temp[0], to_proc);

      send(temp, to_proc * cipher->BLOCK_SIZE);

      input += to_proc * cipher->BLOCK_SIZE;
      blocks -= to_proc;
      }
   }

/*
* Finish encrypting in ECB mode
*/
void ECB_Decryption::buffered_final(const byte input[], u32bit length)
   {
   if(length == 0 || length % cipher->BLOCK_SIZE != 0)
      throw Decoding_Error(name() + ": Ciphertext not multiple of block size");

   size_t extra_blocks = (length - 1) / cipher->BLOCK_SIZE;

   buffered_block(input, extra_blocks * cipher->BLOCK_SIZE);

   input += extra_blocks * cipher->BLOCK_SIZE;

   cipher->decrypt(input, temp);
   send(temp, padder->unpad(temp, cipher->BLOCK_SIZE));
   }

}