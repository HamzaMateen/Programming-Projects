use clap::{value_parser, Arg, Command};

pub fn get_cli() -> Command {
    Command::new("ccut")
        .version("0.1.0")
        .author("Hamza Mateen")
        .about("A cli utility for cutting text out of each line of a line-feed or text file.")
        .arg(
            Arg::new("field")
                .short('f')
                .long("field")
                .help("Specifies the field to be extracted from a line.")
                .value_parser(value_parser!(u32))
                .value_delimiter(','),
        )
        .arg(
            Arg::new("delimeter")
                .short('d')
                .long("delim")
                .default_value("\t")
                .help("Delimieter acts as the word separator in each line."),
        )
        .arg(
            Arg::new("path")
                .short('p')
                .long("path")
                .help("Specifies the path to the line-feed or text-file.")
                .default_value("-"),
        )
}
