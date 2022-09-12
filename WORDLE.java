package com.company;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class WORDLE {

    static ArrayList<Character> charsNotInWord= new ArrayList<Character>();
    static ArrayList<Character> yellowChar= new ArrayList<Character>();
    static ArrayList<Integer> yellowCharNotPos= new ArrayList<Integer>();
    static ArrayList<Character> greenChar= new ArrayList<Character>();
    static ArrayList<Integer> greenCharPos= new ArrayList<Integer>();


    static ArrayList<String> getListOfWords() throws FileNotFoundException {
        Scanner fileReader = new Scanner(new File("C:\\Users\\User\\OneDrive" +
                "\\Documents\\Engineering\\Personal\\allFiveLetterWords.txt"));
        ArrayList<String> list = new ArrayList<String>();
        while (fileReader.hasNext()) {
            list.add(fileReader.next());
        }
        return list;
    }

    static String userInput(ArrayList<String> list) {
        Scanner input = new Scanner(System.in);
        System.out.print("Word Entered: ");
        String word = input.nextLine();
        while (word.length() != 5 || !list.contains(word)) {
            System.out.println("Please enter a valid word");
            System.out.print("Word Entered: ");
            word = input.nextLine();
        }
        System.out.print("Yellow Green or Wrong(Y,G,X): ");
        String colour = input.nextLine();
        while (colour.length() != 5) {
            System.out.println("Please enter a valid colours");
            System.out.print("Yellow Green or Wrong(Y,G,X): ");
            colour = input.nextLine();
        }
        return word + colour;
    }

    static boolean checkIfYellow(String word) {

        for (int k = 0; k < yellowChar.size(); k++){
            if (word.indexOf(yellowChar.get(k)) == -1) {
                return false;
            }
            else{
                continue;
            }
        }
        for (int i = 0; i < word.length(); i++) {
            ArrayList<Integer> internalInt = new ArrayList<Integer>();
            for (int j = 0; j < yellowChar.size(); j++) {
                if (word.charAt(i) == yellowChar.get(j)) {
                    internalInt.add(yellowCharNotPos.get(j));
                }
            }
            if (internalInt.contains(i)) {
                return false;
            } else {
                continue;
            }
        }
        return true;
    }

    static boolean checkIfGreen(String word) {
        for (int i = 0; i < greenChar.size(); i++) {
            if (greenChar.get(i) == word.charAt(greenCharPos.get(i))) {
                continue;
            } else {
                return false;
            }
        }
        return true;
    }

    static boolean checkNotInWord(String word) {
        for (int j = 0; j < word.length(); j++) {
            if (charsNotInWord.contains(word.charAt(j)) &&
                    !greenChar.contains(word.charAt((j)))){
                return false;
            }
        }
        return true;
    }

    static void checkInput(String userInput, ArrayList<String> list) {
        String[] parts = {userInput.substring(0, 5), userInput.substring(5)};
        String word = parts[0];
        String colour = parts[1];

        for (int x = 0; x < 5; x++) {
            if (colour.charAt(x) == 'X') {
                charsNotInWord.add(word.charAt(x));
            } else if (colour.charAt(x) == 'Y') {
                yellowChar.add(word.charAt(x));
                yellowCharNotPos.add(x);
            } else if (colour.charAt(x) == 'G') {
                greenChar.add(word.charAt(x));
                greenCharPos.add(x);
            }
        }
    }


    static void createListOfPossibleWords(ArrayList<String> list) {
        ArrayList<String> updatedList = new ArrayList<String>();
        for (String s : list) {
            if (!checkNotInWord(s)) {
                continue;
            }
            if (checkIfYellow(s) && checkIfGreen(s)) {
                updatedList.add(s);
            }
        }

        System.out.println(updatedList.size() + " options - " + updatedList);
    }


    public static void main(String[] args) throws FileNotFoundException {

        ArrayList<String> list = getListOfWords();
        int count = 0;
        while (count < 6){
            checkInput(userInput(list), list);
            createListOfPossibleWords(list);
            count++;
        }

    }
}