void serial_in()
{
  if (Serial.available()) {
    out = Serial.read();
    Serial.println(out);
  }
}
