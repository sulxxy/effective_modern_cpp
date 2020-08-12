int main(){
    double x = 1;
    double y = 2;
    int z1{x+y};     // warning: -Wnarrowing
    int z2(x+y);     // no warning
}