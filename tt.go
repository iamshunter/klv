package main
import (
      "fmt"
      "hash/crc32"
      "encoding/binary"
      "encoding/hex"

      pb "ris"
       )

func main() {

    var buff = []byte  {
        0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
        0x0F, 0x4C, 0x33, 0x4D, 0x44, 0x4C, 0x01, 0x00,
        0x18, 0xC1, 0x08, 0x00, 0x04, 0x42, 0xA7, 0xE6,
        0x71, 0x50, 0x12, 0x30, 0x06, 0x10, 0x04, 0x00,
        0x06, 0x90, 0x78, 0xC0, 0x04 }

    len := len(buff)
    sum := crc32.ChecksumIEEE(buff)
    fmt.Printf("ieeeSum %08x\n", sum)

    sumBuf := make([]byte, 4)
    binary.BigEndian.PutUint32(sumBuf, sum)

    buff = append(buff, sumBuf...)

    fmt.Printf("Buff\n%s\n", hex.Dump(buff))

    rb := pb.RisBuffer{ 
        Key: 1234,
    }

    fmt.Printf("%d\n", rb.GetKey())

    rb.Key = 5678
    rb.Data = buff[:len]
    rb.Crc = crc32.ChecksumIEEE(rb.Data[:len])
    fmt.Printf("%d\n", rb.GetKey())
    fmt.Printf("Data\n%s\n", hex.Dump(rb.Data))
    fmt.Printf("%x\n", rb.GetCrc() )
}
