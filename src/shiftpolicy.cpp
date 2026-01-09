void shift(int rpm, bool engineState, int& gear) override {
    if (!engineState) return;
    if (rpm >= 3000 && gear < 5) gear++;
    else if (rpm <= 2000 && gear > 1) gear--;
}
