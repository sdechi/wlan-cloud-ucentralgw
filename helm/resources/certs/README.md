All certificates required for deployment should be placed here. Required list of certificates with description:

1. client-cas.pem - list of CAs that are used for client certificates generation
2. rest-ca.pem - CA for restapi and fileuploader endpoints HTTPS connection certificate
3. rest-cert.pem - X509 certificate for restapi and fileuploader endpoints HTTPS connection
4. rest-key.pem - X509 private key for restapi and fileuploader endpoints HTTPS connection
5. websocket-ca.pem - CA for websocket endpoint HTTPS connection certificate
6. websocket-cert.pem - X509 certificate for websocket endpoint HTTPS connection
7. websocket-issuer.pem - intermediate issuer certificate for websocket endpoint HTTPS connection certificate
8. websocket-key.pem - X509 private key for websocket endpoint HTTPS connection

If you need to pass additional CAs, put them into [cas](./cas) directory
