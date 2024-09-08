use core::fmt;

#[derive(Debug)]
pub enum CoreError {
    TCPConnectionError(String),
}

impl fmt::Display for CoreError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            CoreError::TCPConnectionError(msg) => write!(f, "Connection error: {}", msg),
        }
    }
}

#[derive(Debug)]
pub enum ProtocolError {
    ValueNotFound,
    ValueExists,
    ValueNotStored,
}

impl fmt::Display for ProtocolError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            ProtocolError::ValueNotStored => write!(f, "Server said: value not stored"),
            ProtocolError::ValueExists => write!(f, "Server said: value already exists"),
            ProtocolError::ValueNotFound => write!(f, "Server said: value not found"),
        }
    }
}

// Unification
#[derive(Debug)]
pub enum AppError {
    Core(CoreError),
    Protocol(ProtocolError),
}

impl From<CoreError> for AppError {
    fn from(value: CoreError) -> Self {
        AppError::Core(value)
    }
}

impl From<ProtocolError> for AppError {
    fn from(value: ProtocolError) -> Self {
        AppError::Protocol(value)
    }
}
