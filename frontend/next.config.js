/** @type {import('next').NextConfig} */
module.exports = {
  output: "standalone",
  allowedDevOrigins: ["localhost:80"],
  experimental: {
    serverActions: {
      // edit: updated to new key. Was previously `allowedForwardedHosts`
      allowedOrigins: ["web:3000", "localhost:80", "nginx", "*"],
    },
  },
};
