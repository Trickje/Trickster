pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
            step([$class: 'GitHubSetCommitStatusBuilder'])
            echo 'Changed the status to pending on GitHub'
                echo 'Building..'
            }
        }
        stage('Test') {
            steps {
                echo 'Testing..'
            }
        }
        stage('Deploy') {
            steps {

        commitChangeset = sh(returnStdout: true, script: 'git diff-tree --no-commit-id --name-status -r HEAD').trim()
        step([$class: 'GitHubCommitStatusSetter', statusResultSource: [$class: 'ConditionalStatusResultSource', results: []]])
                echo 'Deploying....'
            }
        }
    }
}