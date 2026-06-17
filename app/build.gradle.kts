plugins {
    java
    application
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("com.fasterxml.jackson.core:jackson-databind:2.17.1")
    implementation("com.webull.openapi:webull-openapi-java-sdk:1.0.3")
    implementation("org.slf4j:slf4j-api:2.0.12")
    implementation("org.slf4j:slf4j-simple:2.0.12")

    // Legacy dependencies for Java 11 compatibility
    implementation("javax.xml.bind:jaxb-api:2.3.1")
    implementation("com.sun.xml.bind:jaxb-impl:2.3.3")
}

application {
    mainClass.set("webull.trading.bot.Application")
}

tasks.withType<JavaCompile> {
    options.release.set(21)
}