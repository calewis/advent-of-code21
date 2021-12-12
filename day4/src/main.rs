fn main() {
    let input: Vec<String> = include_str!("input.txt").lines().into_iter().collect();
    let number: Vec<u32> = input[0].split(',').map(|s| s.parse().unwrap()).collect();
}
