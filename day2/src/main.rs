fn main() {
    let input: Vec<Vec<String>> = include_str!("input.txt")
        .lines()
        .map(|w| w.split_whitespace().map(|s| s.to_string()).collect())
        .collect();

    let data: Vec<(String, i32)> = input
        .iter()
        .map(|w| (w[0].clone(), w[1].parse().unwrap()))
        .collect();

    //
    // Part 1
    //
    // let mut depth = 0;
    // let mut displacement = 0;
    // for inst in data {
    //     match (&inst.0[..], inst.1) {
    //         ("forward", d) =>  displacement += d,
    //         ("down", d) =>  depth += d,
    //         ("up", u) =>  depth -= u,
    //         _ => println!("Bad direction!"),
    //     }
    // }
    // println!(
    //     "depth {}, disp {}, product {}",
    //     depth,
    //     displacement,
    //     depth * displacement
    // );

    //
    // Part 2
    //
    let mut depth = 0;
    let mut displacement = 0;
    let mut aim = 0;

    for inst in data {
        match (&inst.0[..], inst.1) {
            ("forward", d) => {
                displacement += d;
                depth += d*aim;
            },
            ("down", d) => aim += d,
            ("up", u) => aim -= u,
            _ => println!("Bad direction!"),
        }
    }

    println!(
        "depth {}, disp {}, product {}",
        depth,
        displacement,
        depth * displacement
    );
}
