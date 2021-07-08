FROM debian:bullseye as builder

# Update existing packages.
RUN apt-get update
RUN apt-get upgrade -y

# Install build tools and dev libraries.
RUN apt-get install -y g++ cmake git libcurl4-openssl-dev libssl-dev

# Create dir to copy sources to
RUN mkdir /src
WORKDIR /src

# Copy sources.
COPY CMakeLists.txt /src/
COPY testdata/ /src/testdata/
COPY *.h /src/
COPY *.cpp /src/

# Configure.
RUN /usr/bin/cmake --config Release -H/src -B/build
# Build.
RUN cmake --build /build --config Release -j 8 --target battlesnake_hungry --target battlesnake_hungry_test --target battlesnake_hungry_test_data
# Test.
RUN cd /build/ && /build/battlesnake_hungry_test

FROM debian:bullseye

# Update existing packages.
RUN apt-get update
RUN apt-get upgrade -y

# Prepare working dir.
RUN mkdir /app
WORKDIR /app

# Copy binary.
COPY --from=builder /build/battlesnake_hungry /app/

# Run.
CMD ["/app/battlesnake_hungry"]
