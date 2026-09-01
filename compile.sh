#!/usr/bin/env bash
set -e

MCU="${1:-atmega162}"
case "$MCU" in
    atmega162|atmega2561) ;;
    *)
        echo "Usage: $0 [atmega2561|atmega162]"
        echo "  atmega2561 - x0xlarge"
        echo "  atmega162  - original x0xb0x CPU (default)"
        exit 1
        ;;
esac

DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$DIR"

make clean
make MCU="$MCU"

echo "Built $DIR/out/${MCU}/NeoB0x_${MCU}.hex"
