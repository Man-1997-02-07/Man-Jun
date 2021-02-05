/* AES 加解密 */
const CryptoJS = require('crypto-js')
/**
 * 加密
 * @param {*} pwd
 * @param {*} aesConfigIv
 * @param {*} aesConfigKey
 */
export function encryptAESPassword (pwd, aesConfigIv, aesConfigKey) {
  const iv = CryptoJS.enc.Utf8.parse(aesConfigIv)
  const key = CryptoJS.enc.Utf8.parse(aesConfigKey)
  const decrypt = CryptoJS.AES.encrypt(pwd, key, {
    iv: iv,
    mode: CryptoJS.mode.CBC,
    padding: CryptoJS.pad.ZeroPadding
  })
  // console.log('CryptoJS.enc', decrypt.ciphertext.toString())
  return decrypt.ciphertext.toString()
}
/**
 * 解密
 * @param {*} text
 * @param {*} aesConfigIv
 * @param {*} aesConfigKey
 */
export function decryptAESPassword (text, aesConfigIv, aesConfigKey) {
  const iv = CryptoJS.enc.Utf8.parse(aesConfigIv)
  const key = CryptoJS.enc.Utf8.parse(aesConfigKey)
  const encryptedHexStr = CryptoJS.enc.Hex.parse(text)
  const srcs = CryptoJS.enc.Base64.stringify(encryptedHexStr)
  const decrypt = CryptoJS.AES.decrypt(srcs, key, {
    iv: iv,
    mode: CryptoJS.mode.CBC,
    padding: CryptoJS.pad.ZeroPadding
  })
  // console.log('decryptedStr', decrypt.toString())
  const decryptedStr = decrypt.toString(CryptoJS.enc.Utf8)
  return decryptedStr
}
