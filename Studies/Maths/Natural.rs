use core::ops;
use std::rc::Rc;

#[derive(Debug, Clone)]
enum Nat {
    O,
    S(Rc<Nat>)
}

fn sum(n: &Nat, m: Nat) -> Nat {
    match n {
        Nat::O => m,
        Nat::S(p) => Nat::S(Rc::new(sum(p, m)))
    }
}

impl Nat {
    fn new(n: u32) -> Nat {
        match n {
            0 => Nat::O,
            _ => Nat::S(Rc::new(Nat::new(n-1)))
        }
    }
    
    fn to_u32(&self) -> u32 {
        match self {
            Nat::O => 0,
            Nat::S(p) => p.to_u32()+1
        }
    }
}

impl ops::Add<Nat> for Nat {
    type Output = Nat;

    fn add(self, rhs: Nat) -> Nat {
        let lhs = self.clone();
        sum(&lhs, rhs)
    }
}

fn main() {
    let n = Nat::new(3) + Nat::new(5);
    println!("{:?}, {}", n, n.to_u32());
}