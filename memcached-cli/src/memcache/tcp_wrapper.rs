// networking logic dwells here
use crate::errors::CoreError;
use std::net::TcpStream;
use std::result;

pub struct TcpStreamWrapper {
    pub stream: TcpStream,
}

impl TcpStreamWrapper {
    pub fn new(host: &String, port: &String) -> result::Result<TcpStreamWrapper, CoreError> {
        let stream = TcpStream::connect(format!("{host}:{port}"));

        let error_msg = "TCP Connection could not be established.".to_string();
        match stream {
            Ok(conn) => Ok(TcpStreamWrapper { stream: conn }),
            Err(_) => Err(CoreError::TCPConnectionError(error_msg)),
        }
    }
}
