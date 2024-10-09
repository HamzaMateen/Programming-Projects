use clap::{Arg, Command};

pub fn get_cli() -> Command {
    Command::new("ccwc")
        .version("0.1.0")
        .author("Hamza Mateen")
        .about("CLI utility for collecting text file/feed stats like line, bytes, characters or word count.")
        .arg_required_else_help(true)
        .arg(
            Arg::new("char")
                .short('c')
                .long("char")
                .num_args(0)
                .help("Include character count in the output.")
        )
        .arg(
            Arg::new("line")
                .short('l')
                .long("line")
                .num_args(0)
                .help("Include line count in the output.")
        )
        .arg(
            Arg::new("word")
                .short('w')
                .long("word")
                .num_args(0)
                .help("Include word count in the output.")
        )
        .arg(
            Arg::new("byte")
                .short('b')
                .long("byte")
                .num_args(0)
                .help("Include byte count in the output.")
        )
        .arg(
            Arg::new("path")
                .short('p')
                .long("path")
                .help("Specifies the path to the line-feed or text-file.")
                .default_value("-")
        )
}
