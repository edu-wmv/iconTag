void printTime(int h, int m, int s) {
  char buffer[6];
  Serial.println("");
  sprintf(buffer, "%02d:%02d:%02d", h, m, s);
  Serial.println(buffer);
}