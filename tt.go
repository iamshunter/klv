package main
import (
      "fmt"
      "hash/crc32"
      "encoding/binary"
      "encoding/hex"
      "net"

      proto "github.com/golang/protobuf/proto"
      pb "ris"
       )

func main() {

    // This is a buffer of data taken from the L3 document as a sample
    var buff = []byte  {
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,
        0x18, 0xC1, 0x08, 0x00, 0x04, 0x42, 0xA7, 0xE6,
        0x71, 0x50, 0x12, 0x30, 0x06, 0x10, 0x04, 0x00,
        0x06, 0x90, 0x78, 0xC0, 0x04 }

    // Calculate the Ethernet checksum on the data
    len := len(buff)
    sum := crc32.ChecksumIEEE(buff)
    fmt.Printf("ieeeSum %08x\n", sum)

    // Append the crc to the end of the buffer
    //   we won't use it but it's an exercise we may need later
    sumBuf := make([]byte, 4)
    binary.BigEndian.PutUint32(sumBuf, sum)
    buff = append(buff, sumBuf...)
    fmt.Printf("Buff with CRC added\n%s\n", hex.Dump(buff))

    // Construct and fill a RIS buffer
    rb := pb.RisBuffer{ }
    rb.Key = 5678
    rb.Data = buff[:len]
    rb.Crc = crc32.ChecksumIEEE(rb.Data[:len])

    // Dump the result
    fmt.Printf("Key: %10d\n", rb.GetKey())
    fmt.Printf("CRC: 0x%x\n", rb.GetCrc() )
    fmt.Printf("Data\n%s", hex.Dump(rb.Data))

    // Put the message in a buffer
    serBuf, err := proto.Marshal(&rb)
    if err != nil {
        fmt.Printf("Error Marshal\n")
    }

    // Make a new protobuf
    unRb := pb.RisBuffer{Key:1, Crc:2}
    fmt.Printf("unRb\n");
    fmt.Printf("Key: %10d\n", unRb.GetKey())
    fmt.Printf("CRC: 0x%x\n", unRb.GetCrc() )
    fmt.Printf("Data\n%s", hex.Dump(unRb.Data))

    // Pull it back out of the buffer
    err = proto.Unmarshal(serBuf, &unRb)
    if err != nil {
        fmt.Printf("Error Unmarshal\n")
    }

    // Dump the result
    fmt.Printf("UNMARSHAL\n");
    fmt.Printf("Key: %10d\n", unRb.GetKey())
    fmt.Printf("CRC: 0x%x\n", unRb.GetCrc() )
    fmt.Printf("Data\n%s", hex.Dump(unRb.Data))

    // Open a socket
    client, err := net.Dial("udp", "127.0.0.1:31013")
    if err != nil {
        fmt.Printf("Error opening client\n")
    }
    defer client.Close()

    fmt.Printf("Serial\n%s", hex.Dump(serBuf))
    for ii:= 0; ii < 10; ii++ {
        client.Write(serBuf)
    }


}
