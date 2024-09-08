use clap::{Arg, ArgAction, Command};

pub fn create_cli() -> Command {
    let cli = clap::Command::new("Memcached Client CLI")
        .author("Hamza Mateen")
        .version("1.0")
        .about("A CLI client for Memcached")
        .disable_help_flag(true);

    // adding different arguments to our cli program now
    cli.arg(
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
    )
}
