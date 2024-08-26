use clap::parser::ValueSource;
use clap::{Arg, ArgAction, ArgMatches, Command, Subcommand};
use core::fmt;
use std::fmt::Formatter;
use std::io::{prelude::*, BufReader};
// use std::net::Ipv4Addr;
use std::net::TcpStream;
use std::time::Duration;

// custom
/*
Authentication
--------------

Optional username/password token authentication (see -Y option). Used by
sending a fake "set" command with any key:

set <key> <flags> <exptime> <bytes>\r\n
username password\r\n

key, flags, and exptime are ignored for authentication. Bytes is the length
of the username/password payload.

- "STORED\r\n" indicates success. After this point any command should work
  normally.

- "CLIENT_ERROR [message]\r\n" will be returned if authentication fails for
  any reason.

*/
fn authenticate(conn: &mut TcpStream, username: &String, passwd: &String) -> std::io::Result<()> {
    let credentials = format!("{username} {passwd}\r\n");
    let len = credentials.len() - 2;

    let command = format!("set placeholder 0 0 {len}\r\n");

    // send the commands to server
    conn.write_all(&command.as_bytes())?;
    conn.write_all(&credentials.as_bytes())?;

    conn.flush()?;

    // set a reading time-out for subsequent read operations.
    conn.set_read_timeout(Some(Duration::from_secs(5)))?;

    // read the response
    let mut output = String::new();
    let mut buf = [0u8; 100];

    loop {
        // reading the response line into this buffer
        let bytes_read = conn.read(&mut buf)?;

        if bytes_read == 0 {
            // end of stream
            println!("Ending here");
            break;
        }

        // take only the valid part, and not the whole 100 bytes
        let chunk = &buf[..bytes_read];
        let chunk_str = String::from_utf8_lossy(chunk).to_owned().to_string();
        output.push_str(&chunk_str[..]);

        println!("chunk: {chunk_str}");

        if output.contains("\r\n") {
            // end stream
            println!("or here");
            break;
        }
    }

    println!("Response: {output}");

    Ok(())
}

fn query_data(stream: &mut TcpStream, keys: &mut [&str]) -> std::io::Result<String> {
    // construct valid commands from the args
    let mut command = String::from("get");

    for key in keys.iter() {
        command.push_str(&format!(" {key}"));
    }
    command.push_str("\r\n");

    // send command to server
    stream
        .write_all(&command.as_bytes())
        .expect("Failed to send 'get' command to server");
    stream.flush()?;

    // read the result
    /*
    After this command, the client expects zero or more items, each of
    which is received as a text line followed by a data block. After all
    the items have been transmitted, the server sends the string

    "END\r\n"

    to indicate the end of response.

    Each item sent by the server looks like this:

    VALUE <key> <flags> <bytes> [<cas unique>]\r\n
    <data block>\r\n
    */

    let reader = BufReader::new(stream);

    for line in reader.lines() {
        let line = line?;
        println!("{line}");
        if line.contains("END") {
            break;
        }
    }

    // return result to user

    Ok("Hello There".to_string())
}

fn connect(host: &String, port: &String) -> std::io::Result<TcpStream> {
    let stream = TcpStream::connect(format!("{host}:{port}"));

    match stream {
        Ok(_) => println!("connection succesful!"),
        Err(_) => println!("Some error occurred!"),
    }

    Ok(stream?)
}

/*
 * Deletion
--------

The command "delete" allows for explicit deletion of items:

delete <key> [noreply]\r\n

- <key> is the key of the item the client wishes the server to delete

- "noreply" optional parameter instructs the server to not send the
  reply.  See the note in Storage commands regarding malformed
  requests.

The response line to this command can be one of:

- "DELETED\r\n" to indicate success

- "NOT_FOUND\r\n" to indicate that the item with this key was not
  found.

See the "flush_all" command below for immediate invalidation
of all existing items.
*/
#[derive(Debug)]
enum MemcachedError {
    ValueNotFound,
    ValueExists,
    ValueNotStored,
}

impl fmt::Display for MemcachedError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            MemcachedError::ValueNotFound => write!(f, "value not found in the cache"),
            MemcachedError::ValueNotStored => write!(f, "value not stored in the cache"),
            MemcachedError::ValueExists => write!(f, "value already exists in the cache"),
        }
    }
}

fn delete(
    stream: &mut TcpStream,
    key: &String,
    noreply: bool,
) -> std::result::Result<(), MemcachedError> {
    let mut command = format!("delete ${key}");
    if noreply {
        command.push_str(" [noreply]");
    }
    command.push_str("\r\n");

    // send command to the server
    stream
        .write_all(command.as_bytes())
        .expect("command not sent");
    stream.flush().expect("couldn't be flushed.");

    let reader = BufReader::new(stream);
    for line in reader.lines() {
        if noreply {
            break;
        }

        let line = line.expect("can't read this line");
        if line.contains("DELETED") {
            break;
        } else if line.contains("NOT_FOUND") {
            return Err(MemcachedError::ValueNotFound);
        }
    }

    Ok(())
}

fn set_key(
    stream: &mut TcpStream,
    key: &String,
    flags: u16,
    exptime: i32,
    bytes: u16,
    noreply: bool,
    data: &String,
) -> std::result::Result<(), MemcachedError> {
    let mut command_1 = format!("set {key} {flags} {exptime} {bytes}");
    if noreply {
        command_1.push_str(" [noreply]");
    }
    command_1.push_str("\r\n");
    let command_2 = format!("{data}\r\n");

    // send commands
    stream.write_all(command_1.as_bytes()).unwrap();
    stream.write_all(command_2.as_bytes()).unwrap();
    stream.flush().expect("Flush operation failed!");

    let reader = BufReader::new(stream);
    for line in reader.lines() {
        let line = line.unwrap();

        if noreply || line.contains("STORED") {
            break;
        } else if line.contains("NOT_STORED") {
            return Err(MemcachedError::ValueNotStored);
        }
    }
    Ok(())
}

fn main() -> std::io::Result<()> {
    let mut program = clap::Command::new("Memcached Client CLI")
        .author("Hamza Mateen")
        .version("1.0")
        .about("A CLI client for Memcached")
        .disable_help_flag(true);

    // adding different arguments to our cli program now
    program = program
        .arg(
            Arg::new("help")
                .long("help")
                .help("Displays help.")
                .action(clap::ArgAction::Help),
        )
        .arg(
            Arg::new("host")
                .short('h')
                .long("host")
                .help("Address of the host which the Memcached server is running on")
                .default_value("localhost"),
        )
        .arg(
            Arg::new("port")
                .short('p')
                .long("port")
                .help("Port which the server is listening on")
                .default_value("11211"),
        )
        .subcommand(
            Command::new("set")
                .about("Set a key-value pair")
                .arg(Arg::new("key").help("Specifies the key").required(true))
                .arg(Arg::new("value").help("Specifies the value").required(true))
                .arg(
                    Arg::new("noreply")
                        .help("Suppress server's response.")
                        .short('n')
                        .long("noreply")
                        .action(ArgAction::SetTrue),
                ),
        )
        .subcommand(
            Command::new("delete")
                .about("Remove a key-value pair")
                .arg(
                    Arg::new("key")
                        .help("Specifies the <key> to be removed")
                        .required(true),
                )
                .arg(
                    // took some good time to learn this bit lol
                    Arg::new("noreply")
                        .help("Suppresses server's response")
                        .short('n')
                        .long("noreply")
                        .action(ArgAction::SetTrue),
                ),
        );

    // using the CLI
    let matches = program.get_matches();

    let host = matches.get_one::<String>("host").unwrap();
    let port = matches.get_one::<String>("port").unwrap();

    println!("host: {}, port: {}", host, port);

    // const HOST_ADDR: Ipv4Addr = Ipv4Addr::new(127, 0, 0, 1);
    let mut stream = connect(&host, &port).unwrap();
    stream.set_read_timeout(Some(Duration::from_secs(10)))?;

    authenticate(&mut stream, &String::from("venom1"), &String::from("code1"))?;

    let mut keys = ["placeholder", "else", "bro"];
    query_data(&mut stream, &mut keys)?;

    /*
    // let's delete some data
    let delete_key: &String;
    let noreply: bool;

    let delete_cmd_matches = matches.subcommand_matches("delete").unwrap();
    delete_key = delete_cmd_matches.get_one::<String>("key").unwrap();

    if let Some(true) = delete_cmd_matches.get_one::<bool>("noreply") {
        noreply = true;
    } else {
        noreply = false;
    }

    match delete(&mut stream, delete_key, noreply) {
        Ok(_) => println!("Delete operation run successfully."),
        Err(e) => println!("DeletionError: {}", e),
    };
    */
    // let's set some keys
    // let set_matches = matches.subcommand_matches("set").unwrap();
    // let setting_key = set_matches.get_one::<String>("key").unwrap();
    // let setting_value = set_matches.get_one::<String>("value").unwrap();
    // let noreply = *set_matches.get_one::<bool>("noreply").unwrap();
    // let data_size = setting_value.chars().count();
    //
    // match set_key(
    //     &mut stream,
    //     setting_key,
    //     0,
    //     0,
    //     data_size as u16,
    //     noreply,
    //     setting_value,
    // ) {
    //     Ok(_) => println!("key-pair set succesfully."),
    //     Err(e) => println!("ValueSetError: {e}"),
    // }
    //
    let mut fetch_these = vec!["name", "mylove"];
    query_data(&mut stream, &mut fetch_these)?;

    Ok(())
}

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */
