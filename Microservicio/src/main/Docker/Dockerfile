From openjdk:11
Volume /tmp
add Sensores-*.jar app.jar

ENTRYPOINT ["java", "-Djava.security.edg=file:/dev/./urandom", "-jar","app.jar"]