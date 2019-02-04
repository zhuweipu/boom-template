// sbt build file
lazy val commonSettings = Seq(
  organization := "edu.berkeley.cs",
  name := "boom-template",
  version := "1.0",
  scalaVersion := "2.12.4",
  traceLevel := 15,
  scalacOptions ++= Seq("-deprecation","-unchecked","-Xsource:2.11"),
  libraryDependencies += "org.scalatest" %% "scalatest" % "3.0.1" % "test",
  libraryDependencies += "org.json4s" %% "json4s-native" % "3.5.3",
  libraryDependencies += "org.scala-lang" % "scala-reflect" % scalaVersion.value,
  addCompilerPlugin("org.scalamacros" % "paradise" % "2.1.0" cross CrossVersion.full),
   resolvers ++= Seq(
     Resolver.sonatypeRepo("snapshots"),
     Resolver.sonatypeRepo("releases"),
     Resolver.mavenLocal))

// setup dependency tree
lazy val rocketchip = RootProject(file("rocket-chip"))
lazy val boom = (project in file("boom")).settings(commonSettings).dependsOn(rocketchip)

// by default SBT sets the root project to ".", instead set the root to boom
onLoad in Global ~= (_ andThen ("project boom" :: _))
